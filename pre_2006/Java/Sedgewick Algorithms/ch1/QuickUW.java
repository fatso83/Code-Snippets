// Weighted version of quick union. 
// See page 17 of Algorithms in Java (Sedgewick)
//import in_out.*;


public class QuickUW
{
    public static void main(String[] args)
    {
	int N = Integer.parseInt(args[0]);
	int id[] = new int[N], sz[] = new int[N];

// Lagt til av meg
// N-1 unike parkoblinger tilsier at alle N er koblet sammen.
	int conMax = N-1;
	int con = 0;

	for (int i = 0; i < N ; i++)
	    { id[i] = i; sz[i] = 1; }

	for( In.init(); !In.empty(); )
	    { 
		int i, j, p = In.getInt(), q = In.getInt();

		for (i = p; i!= id[i]; i =id[i]);
		for (j = q; j!= id[j]; j =id[j]);

		if (i == j) continue;

		if (sz[i] < sz[j])
		    {
			id[i] = j; sz[j] += sz[i]; 
		    }
		else
		    {
			id[j] = i; sz[i] += sz[j];
		    }
		Out.println(" " + p + " " + q);
		
		if ( conMax == ++con )
			return;
		
	    }
    }
}

