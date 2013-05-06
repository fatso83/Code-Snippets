int main()
{
	int i=0;
	char *strings[4];
	int q[][6]= { {2,1,0},
		      {3,2,1,0},
		      {4,3,2,1,0},
		      {5,4,3,2,1,0}
		    };

	//size 3
	strings[0]=	"xxQ\n"\
			"xQx\n"\
			"Qxx\n";
	//size 4
	strings[1]=     "xxxQ\n"\
			"xxQx\n"\
			"xQxx\n"\
			"Qxxx\n";
	//size 5
	strings[2]= 	"xxxxQ\n"\
			"xxxQx\n"\
			"xxQxx\n"\
			"xQxxx\n"\
			"Qxxxx\n";
	//size 6
	strings[3]=	"xxxxxQ\n"\
			"xxxxQx\n"\
			"xxxQxx\n"\
			"xxQxxx\n"\
			"xQxxxx\n"\
			"Qxxxxx\n";
	
	
	while(i<4){
		printf("\ndraw_chessboard(%d)", i+3);
		draw_chessboard(q[i],i+3);
		printf("Egentlig streng:\n%s", strings[i]);
		i++;
	}

	return 0;
}
