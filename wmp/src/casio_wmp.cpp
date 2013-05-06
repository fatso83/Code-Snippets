/**
	This is wmp_manager v0.1 by Florian Schmidt (schmidt_florian at gmx.de)
	based on Casio WMP-1 MMPORT Driver v1.0 - by Jimi
	
	23.01.2005
	...
	06.02.2005 from Carl-Erik Kopseng:
		- fixed usage output
		by Flo
		- new menu entry do dump the complete mmc
		- new menu entry to upload the complete mmc
	17.04.1005 Carl-Erik Kopseng has problems with id3v2 tags
		by flo
		- strip off id3v2 tags on mp3-uploads
	This is free software... see COPYING
*/

#include <stdio.h>
#include <unistd.h>

#include <string>
#include <vector>
#include <list>
#include <map>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <errno.h>
#include <netinet/in.h>  // byte conversion
#include "usb_layer.h"

using namespace std;

double estimated_kbps = 44.20;

#define MIN(a, b) ((a)<(b)?(a):(b))

unsigned char MMC_TOC[16384 * 50];
string MMC_ID = "";
string MMC_OtherInfo = "";
unsigned int MMC_TotalSize = 0;
int MMC_TrackCount = 0;
vector<int> MMC_TrackOffset;
vector<int> MMC_TrackSize;
vector<string> MMC_TrackTitle;
vector<string> MMC_TrackArtist;
vector<string> MMC_TrackAlbum;
vector<string> MMC_TrackAnimation;

map<int, int> MMC_UsedSpace;

int MMC_TotalFree = 0;
int MMC_BiggestFreeBlock = 0;

double get_time() {
	struct timeval tv;
		
	if(gettimeofday(&tv, NULL)) {
		perror("gettimeofday");
		return 0;
	}

	return (double)tv.tv_sec + (double)tv.tv_usec / 1000000.;
}

void print_binary(char* name, unsigned char* data, int length) {
	printf("name: %s\ndata: ", name);
	for(unsigned int i = 0; i < length; i++)
		printf("%02x  ", data[i]);
	printf("\n      ");
	for(unsigned int i = 0; i < length; i++)
		printf("%c   ", isprint(data[i])?data[i]:'.');
	printf("\n      ");
	for(unsigned int i = 0; i < length; i++)
		printf("%-3d ", data[i]);
	printf("\n");
}

void print_binary_text(char* name, unsigned char* data, int length) {
	printf("name: %s\ndata: ", name);
	for(unsigned int i = 0; i < length; i++)
		printf("%c", isprint(data[i])?data[i]:'.');
	printf("\n");
}


int get_filesize(const char* filename) {
	struct stat b;
	
	if(!stat(filename, &b)) {
		perror("stat");
		return -1;
	}
	return b.st_size;
}

// printf("%s:%d in %s()\n", __FILE__, __LINE__, __func__);

int MMC_Read(int device_handle, int Offset, int Length, unsigned char* buffer) {
	int TempNum = Length & 511;
	int TransferLength;

	if(TempNum != 0)
		TransferLength = Length + (512 - TempNum);
	else
		TransferLength = Length;
	
	unsigned char get_setup[16];
	
	memset(get_setup, 0, 16);
	memcpy(get_setup + 4, &Offset, 4);
	Offset += TransferLength;
	memcpy(get_setup + 8, &Offset, 4);
	memcpy(get_setup + 12, &TransferLength, 4);

	int ret = usb_vendor_message_out(device_handle, 1, 1, 0, get_setup, 16);
	if(ret < 0) {
		printf("can't send bulk read control message request!\n");
		return -1;
	}

	// bulk read seems to read to much data!
	int len = usb_bulk_read(device_handle, 0, 130, buffer, TransferLength);

	// printf("got %d bulk bytes\n", len);

	if(len < 0 ) {
		perror("usb_bulk_read");
		printf("MMC Reading failed. returncode: %d\n", len);
		return -1;
	}
	return 0;
}
	
int MMC_Write(int device_handle, int Offset, unsigned char* DataBuffer, int length) {
	int TempNum = length & 511;
	unsigned char* data = NULL;
	int padding = 0;

	if(TempNum)
		padding = 512 - TempNum;

	data = (unsigned char*)malloc(length + padding);
	
	if(!data)
		return 0;
	
	memcpy(data, DataBuffer, length);
	memset(data + length, 0, padding);
	
	length += padding;
	
	/* Get status from Device */
	unsigned char buffer[4];
	int status = usb_vendor_message_in(device_handle, 1, 4, 0, buffer, 4);
	
	/*
	print_binary("status", buffer, 4);
	printf("writing data to offset %d with length %d\n", Offset, length);
	print_binary_text("data", data, length);
	*/
	
	unsigned char put_setup[16];
	memset(put_setup, 0, 16);
	memcpy(put_setup + 4, &Offset, 4);
	Offset += length;
	memcpy(put_setup + 8, &Offset, 4);
	memcpy(put_setup + 12, &length, 4);

	int ret = usb_vendor_message_out(device_handle, 1, 0, 0, put_setup, 16);
	
	if(ret < 0) {
		printf("can't write bulk write control message\n");
		return -1;
	}

	if(usb_bulk_write(device_handle, 0, 1, data, length) < 0) {
		printf("bulk write failed\n");
		free(data);
		return -1;
	}

	free(data);
	return 0;
}
	
int TOC_ReadFromWatch(int device_handle) {
	int ret = MMC_Read(device_handle, 0, 16896, MMC_TOC);

	return ret < 0 ? -1:0;
}

	
void TOC_WriteToWatch(int device_handle) {
	MMC_Write(device_handle, 0, MMC_TOC, 16896);
	return;
}

void TOC_UseSpace(int ItemOffset, int ItemLength) {
	/* Is ItemLength a multiple of 512? */
	int TempNum = ItemLength & 511;
	if(TempNum != 0)
		ItemLength += (512 - TempNum);
	
	MMC_UsedSpace.insert(pair<int, int>(ItemOffset, ItemLength));
	
	return;
}

int TOC_ParseIntoArray() {
	memcpy(&MMC_TrackCount, MMC_TOC + 4, 4);

	int cur_pos = 512;

	MMC_TrackOffset.resize(MMC_TrackCount);
	MMC_TrackSize.resize(MMC_TrackCount);
	MMC_TrackTitle.resize(MMC_TrackCount);
	MMC_TrackArtist.resize(MMC_TrackCount);
	MMC_TrackAlbum.resize(MMC_TrackCount);
	MMC_TrackAnimation.resize(MMC_TrackCount);

	for(int TrackNo = 0; TrackNo < MMC_TrackCount; TrackNo++) {
		// printf("reading track %d\n", TrackNo + 1);
		
		char buffer[128];
		memcpy(&MMC_TrackOffset[TrackNo], MMC_TOC + cur_pos, 4);
		memcpy(&MMC_TrackSize[TrackNo], MMC_TOC + cur_pos + 4, 4);
	
		memcpy(buffer, MMC_TOC + cur_pos + 8, 32);
		MMC_TrackTitle[TrackNo] = buffer;
		
		memcpy(buffer, MMC_TOC + cur_pos + 40, 32);
		MMC_TrackArtist[TrackNo] = buffer;
		
		memcpy(buffer, MMC_TOC + cur_pos + 72, 32);
		MMC_TrackAlbum[TrackNo] = buffer;
		
		// memcpy(buffer, MMC_TOC + cur_pos + 8192, 128);
		
		/* Mark this block as used */
		TOC_UseSpace(MMC_TrackOffset[TrackNo], MMC_TrackSize[TrackNo]);
		cur_pos += 128;
		
		/* We processed 8192 TOC data... */
		if(cur_pos > 8704) {
			printf("warning beyond the end of the toc!\n");
			break;
		}
	}

	return 0;
}
		
void TOC_GenerateFromArray() {
	/* Update counters in TOC-Header */
	memset(MMC_TOC, 0, 4);
	memcpy(MMC_TOC + 4, &MMC_TrackCount, 4);
	memset(MMC_TOC + 8, 0, 2);
	memcpy(MMC_TOC + 10, &MMC_TrackCount, 4);
	memset(MMC_TOC + 14, 0, 2);
	
	for(int i = 0; i < MMC_TrackCount; i++) {
		if(MMC_TrackTitle[i].size() > 31)
			MMC_TrackTitle[i] = MMC_TrackTitle[i].substr(0, 31);
		
		if(MMC_TrackArtist[i].size() > 31)
			MMC_TrackArtist[i] = MMC_TrackArtist[i].substr(0, 31);
		
		if(MMC_TrackArtist[i].size() > 31)
			MMC_TrackAlbum[i] = MMC_TrackAlbum[i].substr(0, 31);
		
		unsigned char* TrackEntry = MMC_TOC + i * 128 + 512;
		
		memset(TrackEntry, 0, 128);
		memcpy(TrackEntry, &MMC_TrackOffset[i], 4);
		memcpy(TrackEntry + 4, &MMC_TrackSize[i], 4);
		memcpy(TrackEntry + 8, MMC_TrackTitle[i].c_str(), MMC_TrackTitle[i].size());
		memcpy(TrackEntry + 40, MMC_TrackArtist[i].c_str(), MMC_TrackArtist[i].size());
		memcpy(TrackEntry + 72, MMC_TrackAlbum[i].c_str(), MMC_TrackAlbum[i].size());
		
		memset(MMC_TOC + MMC_TrackCount * 128 + 512 + i * 128, 0, 128);
	}
	/* Enlarge to get 8192 TOC */
	// not needed MMC_TOC = MMC_TOC || Copies(D2C(0), 128 * (64 - MMC_TrackCount))
	
	// print_binary_text("MMC_TOC", MMC_TOC, 16384);

	/* Enlarge to get 16384 TOC */
	// not needed MMC_TOC = MMC_TOC||Copies(D2C(0),128*(64-MMC_TrackCount))

	return;
}

int TOC_SearchForID(int search_id) {
	for(int i = 0; i < MMC_TrackCount; i++) {
		if(MMC_TrackOffset[i] == search_id)
			return i;
	}

	return 0;
}
	
void TOC_FreeSpace(int ItemOffset) {
	map<int, int>::iterator i = MMC_UsedSpace.find(ItemOffset);

	if(i != MMC_UsedSpace.end())
		MMC_UsedSpace.erase(i);
	else
		printf("%s(): Error, unknown offset!\n", __func__);

	return;
}
	
int TOC_FindSpace(int wish) {
	int start = 16896;

	int best_fit_offset = -1;
	int best_fit = 0;

	for(map<int, int>::iterator i = MMC_UsedSpace.begin(); i != MMC_UsedSpace.end(); i++) {
		int offset = i->first;
		int size = i->second;
	
		int before = offset - start;
	
		if(before >= wish && (before < best_fit || best_fit_offset == -1)) {
			best_fit = before;
			best_fit_offset = start;
		}
	
		start = offset + size;	
	}
	int after = MMC_TotalSize - start;
	if(after >= wish && (after < best_fit || best_fit_offset == -1)) {
		best_fit = after;
		best_fit_offset = start;
	}
	
	return best_fit_offset;
}
	
void TOC_CalcFreeSpace() {
	int start = 16896;

	MMC_BiggestFreeBlock = 0;
	MMC_TotalFree = 0;
	for(map<int, int>::iterator i = MMC_UsedSpace.begin(); i != MMC_UsedSpace.end(); i++) {
		int offset = i->first;
		int size = i->second;
	
		int before = offset - start;
		if(before < 0) {
			printf("before is negative at offset %d\n", offset);
		}
		if(before > MMC_BiggestFreeBlock)
			MMC_BiggestFreeBlock = before;
		
		MMC_TotalFree += before;
		
		start = offset + size;	
	}
	int after = MMC_TotalSize - start;
	if(after > MMC_BiggestFreeBlock)
		MMC_BiggestFreeBlock = after;
	
	MMC_TotalFree += after;
	
	return;
}

void show_contents() {
	printf("current contents:\n");
	for(int i = 0; i < MMC_TrackCount; i++) {
		printf("%4d. ", i + 1);
		printf("Album '%s' ", MMC_TrackAlbum[i].c_str());
		printf("Artist '%s' ", MMC_TrackArtist[i].c_str());
		printf("Title '%s' ", MMC_TrackTitle[i].c_str());
		printf("size %dkB\n", MMC_TrackSize[i] / 1024);
	}
	printf("%dkB free (total: %dkB)\n", MMC_BiggestFreeBlock / 1024, MMC_TotalFree / 1024);
}

int read_line(char* line, int max, FILE* fp = NULL) {
	if(!fp)
		fp = stdin;
	
	if(fgets(line, max, fp) == NULL) {
		return -1;
	}
	char* cp = line + strlen(line) - 1;
	while(*cp == '\n' || *cp == '\r')
		*cp-- = 0;
	
	return 0;
}

void delete_file(int device_handle) {
	if(!MMC_TrackCount) {
		printf("no files to delete!\n");
		return;
	}

	show_contents();
	
	printf("(enter 'all' to delete all files)\n");
	printf("Which track should be deleted (1-%d): ", MMC_TrackCount);

	char line[16];
	read_line(line, 16);

	if(!strcmp(line, "all")) {
		for(unsigned int i = 0; i < MMC_TrackCount; i++) {
			TOC_FreeSpace(MMC_TrackOffset[i]);
		}
		MMC_TrackCount = 0;
		MMC_TrackAlbum.resize(MMC_TrackCount);
		MMC_TrackArtist.resize(MMC_TrackCount);
		MMC_TrackTitle.resize(MMC_TrackCount);
		MMC_TrackSize.resize(MMC_TrackCount);
		MMC_TrackOffset.resize(MMC_TrackCount);
	} else {
		int track_id = atoi(line);
		if(track_id < 1 || track_id > MMC_TrackCount) {
			printf("wrong track number\n");
			return;
		}
	
		TOC_FreeSpace(MMC_TrackOffset[track_id - 1]);
		
		if(track_id != MMC_TrackCount) {
			for(unsigned int i = track_id - 1; i < MMC_TrackCount - 1; i++) {
				MMC_TrackOffset[i] = MMC_TrackOffset[i + 1];
				MMC_TrackSize[i] = MMC_TrackSize[i + 1];
				MMC_TrackTitle[i] = MMC_TrackTitle[i + 1];
				MMC_TrackArtist[i] = MMC_TrackArtist[i + 1];
				MMC_TrackAlbum[i] = MMC_TrackAlbum[i + 1];
			}
		}
		MMC_TrackCount--;
		MMC_TrackAlbum.resize(MMC_TrackCount);
		MMC_TrackArtist.resize(MMC_TrackCount);
		MMC_TrackTitle.resize(MMC_TrackCount);
		MMC_TrackSize.resize(MMC_TrackCount);
		MMC_TrackOffset.resize(MMC_TrackCount);
	}
	
	TOC_GenerateFromArray();
	TOC_WriteToWatch(device_handle);
	TOC_CalcFreeSpace();

	return;
}

unsigned int get_id3v2_len(FILE* fp) {
	int block_size = 1024;
	char* data = (char*) malloc(block_size);
	unsigned int id3len = 0;
	
	while(1) {
		int length = fread(data, 1, block_size, fp);
		if(length == -1) {
			printf("error reading block from file: %s\n", strerror(errno));
			break;
		}
		
		if(length < 3 || strncmp(data, "ID3", 3))
			break; // no ID3v2 header
		
		printf("found ID3v2 header!\n");
		id3len = 10;
		unsigned char flags = *(unsigned char*)(data + 5);
		if(flags & 15) {
			printf("invalid flags field: 0x%02x\n");
			break;
		}
		if(flags & (1<<4)) {
			printf("ID3v2 Tag has a footer\n");
			id3len += 10;
		}
		unsigned int size = ntohl(*((unsigned int*)(data + 6)));
		// printf("got synchsafe id3v2 body-size of 0x%08x bytes\n", size);
		size = (size & 127) + 
			(((size>>8) & 127) << 7) +
			(((size>>16) & 127) << 14) +
			(((size>>24) & 127) << 21);
		// printf("got id3v2 body-size of 0x%08x (%d) bytes\n", size, size);
		id3len += size;
		
		printf("got total id3v2 size of %d bytes\n", id3len);
		
		break;
	}
	free(data);
	// printf("return id3v2 len: %d\n", id3len);
	return id3len;
}

void add_file(int device_handle) {
	printf("Warning: the watch only plays MP3's with a bitrate between 96kbps and 128kbps and a sample frequency of 44100!\n");

	printf("filename to upload: ");

	char filename[256];
	if(fgets(filename, 256, stdin) == NULL) {
		printf("error reading from stdin\n");
		return;
	}
	char* cp = filename + strlen(filename) - 1;
	while(*cp == '\n' || *cp == '\r')
		*cp-- = 0;
	if(filename[0] == 0) {
		printf("aborting.\n");
		return;
	}

	FILE* fp = fopen(filename, "rb");
	if(!fp) {
		printf("could not open file '%s' for reading!\n", filename);
		perror("fopen");
		return;
	}

	struct stat sbuf;
		
	if(fstat(fileno(fp), &sbuf)) {
		printf("could not stat file!\n");
		perror("stat()");
		return;
	}

	int file_size = sbuf.st_size;
	
	unsigned int start_offset = 0;
	
	if(strcasestr(filename, "mp3")) {
		// check to see if it has an id3v2 tag!
		unsigned int id3v2_len = get_id3v2_len(fp);
		start_offset = id3v2_len;
		file_size -= id3v2_len;
	}
	
	printf("file-size: %dkB (estimated time: %.0fs)\n", 
		file_size / 1024,
		(double)file_size / estimated_kbps / 1024.
	);

	if(file_size == 0) {
		printf("ignoring request to upload empty file!\n");
		return;
	}

	int new_offset = TOC_FindSpace(file_size);

	if(new_offset == -1) {
		printf("im sorry, but there isn't enough space left!\n");
		return;
	}

	string title = filename;
	string album;
	string artist;
	
	string::size_type dpos;
	
	// strip of \.mp3.*
	dpos = title.find(".mp3");
	if(dpos != string::npos)
		title = title.substr(0, dpos);
	
	// split "artist - title"
	dpos = title.find(" - ");
	if(dpos != string::npos) {
		artist = title.substr(0, dpos);
		title = title.substr(dpos + 3);
	}

	char line[256];

	printf("title of new file [%s]: ", title.c_str());
	read_line(line, 256);
	if(line[0])
		title = line;
	
	printf("artist of new file [%s]: ", artist.c_str());
	read_line(line, 256);
	artist = line;
	
	printf("album of new file: ");
	read_line(line, 256);
	album = line;

	MMC_TrackCount++;
	MMC_TrackOffset.push_back(new_offset);
	MMC_TrackSize.push_back(file_size);
	MMC_TrackTitle.push_back(title);
	MMC_TrackArtist.push_back(artist);
	MMC_TrackAlbum.push_back(album);
	
	TOC_UseSpace(new_offset, file_size);
	
	printf("reading file...");
	fflush(stdout);
	
	// seek to mp3 start
	if(fseek(fp, start_offset, SEEK_SET)) {
		printf("could not seek to mp3 start at offset %ld: %s\n", 
			start_offset, strerror(errno)
		);
		return;
	}
	
	unsigned char* data = (unsigned char*) malloc(file_size);
	int length = fread(data, 1, file_size, fp);
	
	printf("done.\n");
	fclose(fp);

	if(length != file_size) {
		printf("Error: could not completely read file!\n");
		return;
	}

	printf("sending file to watch...\n");
	fflush(stdout);

	double start = get_time();
	
	
	unsigned int block_size = 10 * 1024;
	unsigned int file_offset = 0;
	
	while(1) {
		unsigned int len = min(block_size, file_size - file_offset);
		printf("writing to offset %d %d bytes\n", new_offset, len);
		if(MMC_Write(device_handle, new_offset, data + file_offset, len)) {
			printf("error uploading file.\n");
			free(data);
			return;
		}
		file_offset += len;
		new_offset += len;
		if(file_offset == file_size)
			break;
		usleep(100000);
	}
	
	
	double duration = get_time() - start;
	estimated_kbps = (double)file_size / duration / 1024.;
	printf("done. needed %.2f seconds (%.2fkB/s)\n", duration, estimated_kbps);


	printf("updating toc...");
	fflush(stdout);

	TOC_GenerateFromArray();
	usleep(500000);
	TOC_WriteToWatch(device_handle);
	TOC_CalcFreeSpace();
	
	printf("done\n");

	return;
}

void write_from_file(int device_handle) {
	printf("filename to write: ");

	char filename[256];
	if(fgets(filename, 256, stdin) == NULL) {
		printf("error reading from stdin\n");
		return;
	}
	char* cp = filename + strlen(filename) - 1;
	while(*cp == '\n' || *cp == '\r')
		*cp-- = 0;
	if(filename[0] == 0) {
		printf("aborting.\n");
		return;
	}

	FILE* fp = fopen(filename, "rb");
	if(!fp) {
		printf("could not open file '%s' for reading!\n", filename);
		perror("fopen");
		return;
	}

	struct stat sbuf;
		
	if(fstat(fileno(fp), &sbuf)) {
		printf("could not stat file!\n");
		perror("stat()");
		return;
	}

	int file_size = sbuf.st_size;

	printf("file-size: %dkB (estimated time: %.0fs)\n", 
		file_size / 1024,
		(double)file_size / estimated_kbps / 1024.
	);

	if(file_size == 0) {
		printf("ignoring request to upload empty file!\n");
		return;
	}

	unsigned int block_size = 100 * 1024;
	unsigned char* data = (unsigned char*) malloc(block_size);
	if(!data) {
		printf("not enough memory!\n");
		return;
	}

	printf("sending file to watch...");
	fflush(stdout);

	double start = get_time();

	unsigned int remaining_size;
	unsigned int offset = 0;
	unsigned int block_count = 0;
	while(offset < file_size) {
		block_count++;
		printf("\rwriting to mmc...block %d/%d...", block_count, (int)ceil((double)file_size / (double)block_size));
		fflush(stdout);
	
		remaining_size = MIN(file_size - offset, block_size);
	
	
		int length = fread(data, 1, remaining_size, fp);
		if(length != remaining_size) {
			printf("got only %d bytes from file, wanted %d at offset %d\n",
				length, remaining_size, offset);
			return;
		}
	
		if(MMC_Write(device_handle, offset, data, remaining_size)) {
			printf("error uploading file.\n");
			free(data);
			return;
		}
		usleep(150000);
		offset += remaining_size;
	}
	double duration = get_time() - start;
	estimated_kbps = (double)file_size / duration / 1024.;
	printf("done. needed %.2f seconds (%.2fkB/s)\n", duration, estimated_kbps);
	
	fclose(fp);

	printf("because of i dont reread the toc here, i have to exit now!\n");
	exit(1);

	return;
}

void format_reagion(int device_handle) {
	char line[256];
	
	printf("start address (0-%d): ", MMC_TotalSize);
	read_line(line, 64);
	unsigned int start = atoi(line);

	
	printf("laenge (0-%d): ", MMC_TotalSize - start);
	read_line(line, 64);
	unsigned int block_size = atoi(line);
	
	unsigned char* data = (unsigned char*) malloc(block_size);
	
	if(!data) {
		printf("not enough memory!\n");
		return;
	}

	memset(data, 0, block_size);
	
	printf("writing to mmc...");
	fflush(stdout);

	if(MMC_Write(device_handle, start, data, block_size)) {
		printf("error formatting reagion.\n");
		free(data);
		return;
	}

	printf("done.\n");
	
	return;
}

void get_file(int device_handle) {
	if(!MMC_TrackCount) {
		printf("no files to download!\n");
		return;
	}

	show_contents();
	
	printf("which track should be downloaded (1-%d): ", MMC_TrackCount);

	char line[256];
	read_line(line, 16);

	int track_id = atoi(line);
	if(track_id < 1 || track_id > MMC_TrackCount) {
		printf("wrong track number\n");
		return;
	}
	track_id--;

	string filename = MMC_TrackArtist[track_id];

	if(MMC_TrackAlbum[track_id].size()) {
		filename += " (";
		filename += MMC_TrackAlbum[track_id];
		filename += ")";
	}
	filename += " - ";
	filename += MMC_TrackTitle[track_id];

	printf("what filename should be used [%s]: ", filename.c_str());
	read_line(line, 256);

	if(line[0])
		filename = line;

	FILE* fp = fopen(filename.c_str(), "wb");
	if(!fp) {
		printf("could not open file '%s' for writing!\n", filename.c_str());
		perror("fopen");
		return;
	}

	
	int file_size = MMC_TrackSize[track_id];

	printf("file-size: %dkB (estimated time: %.0fs)\n", 
		file_size / 1024,
		(double)file_size / estimated_kbps / 1024.
	);

	unsigned char* data = (unsigned char*) malloc(file_size);

	if(!data) {
		printf("error: not enough memory!\n");
		return;
	}

	printf("downloading file...");
	fflush(stdout);

	double start = get_time();
	MMC_Read(device_handle, MMC_TrackOffset[track_id], file_size, data);
	double duration = get_time() - start;
	printf("done. needed %.2f seconds (%.2fkB/s)\n", duration, (double)file_size / duration / 1024.);
	
	printf("writing file...");
	fflush(stdout);

	if(fwrite(data, file_size, 1, fp) != 1) {
		printf("error writing file\n");
		perror("fwrite");
		free(data);
		return;
	}

	fclose(fp);

	printf("done\n");
	
	free(data);
	
	return;
}

void dump_to_file(int device_handle) {
	string filename = "wmp1.mmc.dump";
	printf("what filename should be used [%s]: ", filename.c_str());
	char line[512];
	read_line(line, 256);

	if(line[0])
		filename = line;

	FILE* fp = fopen(filename.c_str(), "wb");
	if(!fp) {
		printf("could not open file '%s' for writing!\n", filename.c_str());
		perror("fopen");
		return;
	}

	int file_size = MMC_TotalSize;

	printf("file-size: %dkB (estimated time: %.0fs)\n", 
		file_size / 1024,
		(double)file_size / estimated_kbps / 1024.
	);

	// use 100kB Blocks (~2s per block)
	unsigned int block_size = 100 * 1024;
	unsigned char* data = (unsigned char*) malloc(block_size);

	if(!data) {
		printf("error: not enough memory!\n");
		return;
	}

	printf("dumping mmc...");
	fflush(stdout);

	double start = get_time();
	unsigned int offset = 0;
	unsigned int block_count = 0;
	unsigned int remaining_size = 0;
	while(offset < file_size) {
		block_count++;
		printf("\rdumping mmc...block %d/%d...", block_count, (int)ceil((double)file_size / (double)block_size));
		fflush(stdout);
	
		remaining_size = MIN(file_size - offset, block_size);
	
		if(MMC_Read(device_handle, offset, remaining_size, data)) {
			printf("got read error!\n");
			free(data);
			return;
		}
			
		if(fwrite(data, remaining_size, 1, fp) != 1) {
			printf("error writing file\n");
			perror("fwrite");
			free(data);
			return;
		}
		offset += remaining_size;
	}
	double duration = get_time() - start;
	printf("done. needed %.2f seconds (%.2fkB/s)                  \n", duration, (double)file_size / duration / 1024.);
	
	fclose(fp);
	free(data);
	
	return;
}

int show_main_menu() {
	printf(
		"  1) show contents	  6) dump all mmc contents to file\n"
		"  2) delete a file	  7) write a file direct to the mmc\n"
		"  3) upload a new file	  8) format a mmc reagion to 0\n"
		"  4) download a file\n"
		"  5) exit\n"
		"select one option [1-5]: "
	);
	
	char line[16];
	read_line(line, 16);

	printf("\n");

	return atoi(line);
}

int init_device() {
	memset(MMC_TOC, 0, 16384);
	
	/* Open and initialize device */
	int device_handle = wmp_usb_open(0x07CF, 0x3801, 0);
	
	if(device_handle == -1) {
		printf("can't find wmp-1!\n");
		return -1;
	}
	
	unsigned char status[4];
	int ret = usb_vendor_message_in(device_handle, 1, 4, 0, status, 4);

	if(status[1] == 0) {
		/* Device needs to get setup... */
		printf("setting configuration...\n");
		if(usb_device_set_configuration(device_handle, 1)) {
			printf("Error: Failed to setup device\n");
			return -1;
		}
		while(1) {
			usleep(250000);
			usb_vendor_message_in(device_handle, 1, 4, 0, status, 4);
			if(status[1] != 0)
				break;
		}
	}

	if(status[1] == 2) {
		printf("Watch needs to connect to dongle...");
		fflush(stdout);
		usb_vendor_message_in(device_handle, 1, 11, 0, status, 1);
		if(status[0] != 1) {
			printf("Error: Connecting to watch failed\n");
			return -1;
		}
		printf("done.\n");
	
		printf("Connected, waiting for watch to settle...");
		fflush(stdout);
		int settle_timeout = 20;
		while(1) {
			usleep(250000);
			printf(".");
			fflush(stdout);
		
			unsigned char rc[4];
			usb_vendor_message_in(device_handle, 1, 4, 0, rc, 4);
		
			if(rc[0] == 3 && rc[1] == 3 && rc[2] == 0 && rc[3] == 0)
				break;
			
			settle_timeout--;
			
			if(settle_timeout == 0) {
				printf("Error: Watch does not settle - unable to connect\n");
				return -1;
			}
		}
		printf("done.\n");
	}
	
	unsigned char MMC_ID[16];
	ret = usb_vendor_message_in(device_handle, 1, 9, 0, MMC_ID, 16);
	if(ret < 0) {
		printf("Error: can't read mmc_id - is the cradle closed?\n");
		return -1;
	}
		
	if(!strncmp((char*)MMC_ID + 3, "32M", 3))
		MMC_TotalSize = 32047104;
	else {
		printf("Error: unknown MMC-Size\n");
		return -1;
	}
	
	if(TOC_ReadFromWatch(device_handle)) {
		printf("can't read toc from watch!\n");
		return -1;
	}

	if(TOC_ParseIntoArray()) {
		printf("can't parse toc\n");
		return -1;
	}

	TOC_CalcFreeSpace();

	return device_handle;
}

struct upload_job {
	string title;
	string artist;
	string filename;
	int file_size;

	upload_job() {
		file_size = -1;
	}

	upload_job(string title, string artist, string filename) {
		this->title = title;
		this->artist = artist;
		set_filename(filename);
	}
	void set_filename(string filename) {
		this->filename = filename;
	
		struct stat sbuf;
		if(stat(filename.c_str(), &sbuf)) {
			perror("stat");
			return;
		}
		file_size = sbuf.st_size;
	}
};

string basename(string input) {
	string::size_type i = input.rfind("/");
	if(i == string::npos)
		return input;
	return input.substr(i + 1);
}

int process_playlist(int device_handle, char* filename, bool add = false, bool randomize = false, bool song_toc = false, char* convert_command = NULL) {
	list<upload_job> upload_jobs;

	FILE* fp = fopen(filename, "rb");

	if(!fp) {
		printf("error: could not open playlist '%s'\n", filename);
		perror("fopen");
		return -1;
	}

	char line[1024];

	string artist, title;
	int total_size = 0;
	while(read_line(line, 1024, fp) != -1) {
		if(line[0] == '#') {
			if(!strncmp(line, "#EXTINF:", 8)) {
				// read extended info
				title = line;
				int i;
				if((i = title.find(" - ")) != string::npos) {
					artist = title.substr(0, i);
					title = title.substr(i + 3);
					i = artist.find(",");
					artist = artist.substr(i + 1);
				}
			}
			continue;
		}
	
		if(title == "") // no extinf
			title = basename(line);
		
		// process file
		
		upload_job uj = upload_job(title, artist, line);
		total_size += uj.file_size;
		upload_jobs.push_back(uj);
		
		artist = "";
		title = "";
	}
	fclose(fp);

	if(randomize) {
		vector<upload_job> ruj;
	
		ruj.resize(upload_jobs.size());
		srand(time(NULL));
	
		for(list<upload_job>::iterator li = upload_jobs.begin(); li != upload_jobs.end(); li++) {
			int i;
			do
				i = rand() % upload_jobs.size();
			while(ruj[i].file_size != -1);
			ruj[i] = *li;
		}
		upload_jobs.resize(0);
		for(unsigned int i = 0; i < ruj.size(); i++) {
			upload_jobs.push_back(ruj[i]);
		}
	}

	if(convert_command) {
		// convert it....
		total_size = 0;
		int cnt = 1;
		for(list<upload_job>::iterator i = upload_jobs.begin(); i != upload_jobs.end(); i++) {
			upload_job& uj = *i;
			char cmd[512];
			char output[32];
			snprintf(output, 32, "converted_%d.mp3", cnt++);
			snprintf(cmd, 512, convert_command, uj.filename.c_str(), output);
			printf("converting %s...", uj.filename.c_str()); fflush(stdout);
			int retval = system(cmd);
			printf("done.\n");
			if(retval != 0) {
				printf("Warning: got retval: %d\n", retval);
				printf("convert command: '%s'\n", cmd);
				sleep(2);
			}
			uj.set_filename(output);
			total_size += uj.file_size;
			if(total_size > 32 * 1024 * 1024) {
				printf("stopping conversion after %.3fMB of data\n",
					(double)total_size / 1024. / 1024.);
				// remove rest of the playlist
				list<upload_job>::iterator k = i;
				k++;
				for(; k != upload_jobs.end(); k++) {
					upload_jobs.erase(k);
					k = i;
					k++;
				}
				break;
			}
		}
	}
	

	int eta_size = total_size;
	if(total_size > MMC_TotalSize) {
		printf("Warning: your playlist contains %.0fMB. only %.0fMB will fit on the watch!\n"
			"but each track from the playlist is tried...\n",
			(double)total_size / 1024. / 1024.,
			(double)MMC_TotalSize / 1024. / 1024.);
		eta_size = MMC_TotalSize;
	}

	double t = (double)eta_size / 1024. / estimated_kbps;
	printf("%d files. total size: %.2fMB. estimated time: %02.0f:%04.1fs\n",
		upload_jobs.size(),
		(double)total_size / 1024. / 1024.,
		floor(t / 60.),
		t - floor(t / 60.) * 60.
	);

	if(add == false) {
		// remove existing contents
		printf("deleting existing tracks\n");
		for(unsigned int i = 0; i < MMC_TrackCount; i++) {
			TOC_FreeSpace(MMC_TrackOffset[i]);
		}
		MMC_TrackCount = 0;
		MMC_TrackAlbum.resize(MMC_TrackCount);
		MMC_TrackArtist.resize(MMC_TrackCount);
		MMC_TrackTitle.resize(MMC_TrackCount);
		MMC_TrackSize.resize(MMC_TrackCount);
		MMC_TrackOffset.resize(MMC_TrackCount);
	}

	// upload files
	int done = 0;
	for(list<upload_job>::iterator i = upload_jobs.begin(); i != upload_jobs.end(); i++) {
		upload_job& uj = *i;
	
		double t = (double)(total_size - done) / 1024. / estimated_kbps;
	
		printf("%4.1f%% (eta: %02.0f:%04.1fs) reading %s...", 
			(double)done / (double) total_size * 100.,
			floor(t / 60.),
			t - floor(t / 60.) * 60.,
			uj.filename.c_str()
		);
		fflush(stdout);
	
		done += uj.file_size;
		FILE* fp = fopen(uj.filename.c_str(), "rb");
		if(!fp) {
			printf("could not open file '%s' for reading!\n", filename);
			perror("fopen");
			continue;
		}
	
		int file_size = uj.file_size;
		
		unsigned int start_offset = 0;
		
		if(strcasestr(uj.filename.c_str(), "mp3")) {
			// check to see if it has an id3v2 tag!
			unsigned int id3v2_len = get_id3v2_len(fp);
			start_offset = id3v2_len;
			file_size -= id3v2_len;
		}
		
	
		printf("size: %dkB eta: %.0fs...", 
			file_size / 1024,
			(double)file_size / estimated_kbps / 1024.
		);
		fflush(stdout);
	
		if(file_size == 0) {
			printf("ignoring empty file!\n");
			continue;
		}
	
		int new_offset = TOC_FindSpace(file_size);
	
		if(new_offset == -1) {
			printf("not enough space left!\n");
			continue;
		}
	
		MMC_TrackCount++;
		MMC_TrackOffset.push_back(new_offset);
		MMC_TrackSize.push_back(file_size);
		MMC_TrackTitle.push_back(uj.title);
		MMC_TrackArtist.push_back(uj.artist);
		MMC_TrackAlbum.push_back("");
		
		TOC_UseSpace(new_offset, file_size);
		
		// seek to mp3 start
		if(fseek(fp, start_offset, SEEK_SET)) {
			printf("could not seek to mp3 start at offset %ld: %s\n", 
				start_offset, strerror(errno)
			);
			continue;
		}
		
		unsigned char* data = (unsigned char*) malloc(file_size);
		int length = fread(data, 1, file_size, fp);
		fclose(fp);
	
		if(length != file_size) {
			printf("Error: could not completely read file!\n");
			continue;
		}
	
		printf("sending...");
		fflush(stdout);
	
		double start = get_time();
		if(MMC_Write(device_handle, new_offset, data, length)) {
			printf("error uploading file.\n");
			free(data);
			continue;
		}
		double duration = get_time() - start;
		estimated_kbps = (double)file_size / duration / 1024.;
	
		TOC_GenerateFromArray();
		TOC_CalcFreeSpace();
		if(song_toc) {
			printf("toc...");
			fflush(stdout);
		
			usleep(250000);
			TOC_WriteToWatch(device_handle);
		}
		usleep(250000);
		printf("done\n");
	}
	if(!song_toc) {
		printf("writing toc...");
		fflush(stdout);
	
		usleep(250000);
		TOC_GenerateFromArray();
		TOC_WriteToWatch(device_handle);
		TOC_CalcFreeSpace();
		printf("done\n");
	}
	return 0;
}

int main(int argc, char* argv[]) {
	printf("Casio WMP-1 Manager v0.1 by Florian Schmidt (schmidt_florian at gmx.de)\n(based on Casio WMP-1 MMPORT Driver v1.0 - by Jimi)\n\n");
	
	char* playlist = NULL;
	bool randomize = false;
	bool add = false;
	bool song_toc = false;
	char* convert_command = NULL;

	for(unsigned int i = 1; i < argc; i++) {
		if(argv[i][0] == '-') {
			switch(argv[i][1]) {
			case 'h':
				printf("usage: %s [-h] [-r] [-a] [-t] [-c COMMAND] [m3u playlist]\n"
					"\tthe program starts in interactive mode if no playlist is given.\n"
					"\t-r\tthe playlist will be randomized before uploaded\n"
					"\t-t\twrite TOC after each song\n"
					"\t-a\tadd playlist to existing tracks (do not delete existing tracks)\n",
					argv[0]
				);
				return 0;
			case 'r':
				randomize = true;
				break;
			case 'a':
				add = true;
				break;
			case 't':
				song_toc = true;
				break;
			case 'c':
				convert_command = argv[i + 1];
				i++;
				break;
			}
		} else
			if(!playlist)
				playlist = argv[i];
	}

	int device_handle = init_device();
	
	if(device_handle < 0)
		return 1;
	
	
	if(playlist) {
		// playlist mode
		return process_playlist(device_handle, playlist, add, randomize, song_toc, convert_command);
	}

	// interactive mode
	show_contents();

	printf("\n");

	while(1) {
		int selection = show_main_menu();
		
		switch(selection) {
			case 1: // show contents
				show_contents();
				break;
			case 2: // delete file
				delete_file(device_handle);
				break;
			case 3: // add file
				add_file(device_handle);
				break;
			case 4: // get file
				get_file(device_handle);
				break;
			case 5: // exit
				printf("bye!\n\n");
				// todo close device
				return 0;
			case 6:
				dump_to_file(device_handle);
				break;
			case 7:
				write_from_file(device_handle);
				break;
			case 8:
				format_reagion(device_handle);
				break;
		}
		printf("\n");
	}

	/*
	When MMPORT_Command="SRTA" Then Do
	   Parse Value MMPORT_CommandData With SortID ' ' RelativeID
	
	   if SortID=RelativeID Then Do
	      say "DIAG Duplicate IDs"
	      exit
	   End
	
	// Search for the IDs 
	   EntryPos    = TOC_SearchForID(SortID)
	   RelativePos = TOC_SearchForID(RelativeID)
	   if (EntryPos=0) | (RelativePos=0) Then Do
	      say "DIAG Unknown ID"
	      exit
	   End
	
	   if EntryPos=RelativePos+1 Then Do
	      say "DIAG Already at that position"
	      exit
	   End
	
		// Save Entry that is getting sorted
	   MMC_TrackOffset.0    = MMC_TrackOffset.EntryPos
	   MMC_TrackSize.0      = MMC_TrackSize.EntryPos
	   MMC_TrackTitle.0     = MMC_TrackTitle.EntryPos
	   MMC_TrackArtist.0    = MMC_TrackArtist.EntryPos
	   MMC_TrackAlbum.0     = MMC_TrackAlbum.EntryPos
	   MMC_TrackAnimation.0 = MMC_TrackAnimation.EntryPos
	
	   if RelativePos>EntryPos Then Do
		// New position is above old position
		NewPos    = RelativePos
	      SourcePos = EntryPos+1
	      DestinPos = EntryPos
	      Do Forever
		 if DestinPos=NewPos Then
		    Leave
		 MMC_TrackOffset.DestinPos    = MMC_TrackOffset.SourcePos
		 MMC_TrackSize.DestinPos      = MMC_TrackSize.SourcePos
		 MMC_TrackTitle.DestinPos     = MMC_TrackTitle.SourcePos
		 MMC_TrackArtist.DestinPos    = MMC_TrackArtist.SourcePos
		 MMC_TrackAlbum.DestinPos     = MMC_TrackAlbum.SourcePos
		 MMC_TrackAnimation.DestinPos = MMC_TrackAlbum.SourcePos
		 SourcePos = SourcePos+1
		 DestinPos = DestinPos+1
	      End
	   End
	     else Do
	      //* New position is below old position
	      NewPos    = RelativePos+1
	      SourcePos = EntryPos-1
	      DestinPos = EntryPos
	      Do Forever
		 if DestinPos=NewPos Then
		    Leave
		 MMC_TrackOffset.DestinPos    = MMC_TrackOffset.SourcePos
		 MMC_TrackSize.DestinPos      = MMC_TrackSize.SourcePos
		 MMC_TrackTitle.DestinPos     = MMC_TrackTitle.SourcePos
		 MMC_TrackArtist.DestinPos    = MMC_TrackArtist.SourcePos
		 MMC_TrackAlbum.DestinPos     = MMC_TrackAlbum.SourcePos
		 MMC_TrackAnimation.DestinPos = MMC_TrackAlbum.SourcePos
		 SourcePos = SourcePos-1
		 DestinPos = DestinPos-1
	      End
	   End
	
	   //* Now set the actual entry to the new position
	   MMC_TrackOffset.NewPos    = MMC_TrackOffset.0
	   MMC_TrackSize.NewPos      = MMC_TrackSize.0
	   MMC_TrackTitle.NewPos     = MMC_TrackTitle.0
	   MMC_TrackArtist.NewPos    = MMC_TrackArtist.0
	   MMC_TrackAlbum.NewPos     = MMC_TrackAlbum.0
	   MMC_TrackAnimation.NewPos = MMC_TrackAnimation.0
	
	   //* Finally update TOC 
	   call TOC_GenerateFromArray
	   call TOC_WriteToWatch ""
	   say "DIAG Sorting successful"
	 End
	 When MMPORT_Command="SRTB" Then Do
	   Parse Value MMPORT_CommandData With SortID ' ' RelativeID ' '
	
	   if SortID=RelativeID Then Do
	      say "DIAG Duplicate IDs"
	      exit
	   End
	
	   //* Search for the IDs 
	   EntryPos    = TOC_SearchForID(SortID)
	   RelativePos = TOC_SearchForID(RelativeID)
	   if (EntryPos=0) | (RelativePos=0) Then Do
	      say "DIAG Unknown ID"
	      exit
	   End
	
	   if EntryPos=RelativePos-1 Then Do
	      say "DIAG Already at that position"
	      exit
	   End
	
	   //* Save Entry that is getting sorted
	   MMC_TrackOffset.0    = MMC_TrackOffset.EntryPos
	   MMC_TrackSize.0      = MMC_TrackSize.EntryPos
	   MMC_TrackTitle.0     = MMC_TrackTitle.EntryPos
	   MMC_TrackArtist.0    = MMC_TrackArtist.EntryPos
	   MMC_TrackAlbum.0     = MMC_TrackAlbum.EntryPos
	   MMC_TrackAnimation.0 = MMC_TrackAnimation.EntryPos
	
	   if RelativePos>EntryPos Then Do
	      //* New position is above old position
	      NewPos    = RelativePos-1
	      SourcePos = EntryPos+1
	      DestinPos = EntryPos
	      Do Forever
		 if DestinPos=NewPos Then
		    Leave
		 MMC_TrackOffset.DestinPos    = MMC_TrackOffset.SourcePos
		 MMC_TrackSize.DestinPos      = MMC_TrackSize.SourcePos
		 MMC_TrackTitle.DestinPos     = MMC_TrackTitle.SourcePos
		 MMC_TrackArtist.DestinPos    = MMC_TrackArtist.SourcePos
		 MMC_TrackAlbum.DestinPos     = MMC_TrackAlbum.SourcePos
		 MMC_TrackAnimation.DestinPos = MMC_TrackAlbum.SourcePos
		 SourcePos = SourcePos+1
		 DestinPos = DestinPos+1
	      End
	   End
	     else Do
	      //New position is below old position 
	      NewPos    = RelativePos
	      SourcePos = EntryPos-1
	      DestinPos = EntryPos
	      Do Forever
		 if DestinPos=NewPos Then
		    Leave
		 MMC_TrackOffset.DestinPos    = MMC_TrackOffset.SourcePos
		 MMC_TrackSize.DestinPos      = MMC_TrackSize.SourcePos
		 MMC_TrackTitle.DestinPos     = MMC_TrackTitle.SourcePos
		 MMC_TrackArtist.DestinPos    = MMC_TrackArtist.SourcePos
		 MMC_TrackAlbum.DestinPos     = MMC_TrackAlbum.SourcePos
		 MMC_TrackAnimation.DestinPos = MMC_TrackAlbum.SourcePos
		 SourcePos = SourcePos-1
		 DestinPos = DestinPos-1
	      End
	   End
	
	   //* Now set the actual entry to the new position
	   MMC_TrackOffset.NewPos    = MMC_TrackOffset.0
	   MMC_TrackSize.NewPos      = MMC_TrackSize.0
	   MMC_TrackTitle.NewPos     = MMC_TrackTitle.0
	   MMC_TrackArtist.NewPos    = MMC_TrackArtist.0
	   MMC_TrackAlbum.NewPos     = MMC_TrackAlbum.0
	   MMC_TrackAnimation.NewPos = MMC_TrackAnimation.0
	
	   //* Finally update TOC
	   call TOC_GenerateFromArray
	   call TOC_WriteToWatch ""
	 End
	 When MMPORT_Command="FLSH" Then Do
	  call TOC_WriteToWatch "FORCED"
	  say "DIAG flushing content"
	 End
	 Otherwise Do
	  say "NSUP"
	 End
	End
*/
	return 0;
}
