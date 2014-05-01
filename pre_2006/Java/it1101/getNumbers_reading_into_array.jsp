<% 

int antall=10;
String parameterPrefiks = "tall"; //alle variabelnavnene starter på tall. Eks. tall1, tall2, ...
int tall[] = new int[antall];

//putt dataene vi sendte fra siste html-side inn i tallmatrisen vår
for( int i=0; i<tall.length; i++ ){
	//henter variabelen 
	innlest = request.getParameter(parameterPrefiks+i);
	//omgjør den fra streng til heltall, og putter den i matrisen på plass "i"
	tall[i] = Integer.parseInt(innlest);	
}


%>
	
