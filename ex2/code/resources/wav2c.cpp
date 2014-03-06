//////////////////////////////////////////////////////////////////////////////
// wav2c.cpp                                                                //
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// includes                                                                 //
//////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <string.h>

//////////////////////////////////////////////////////////////////////////////
// main                                                                     //
//////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
	printf("wav2c v1.00 by Dark Fader / BlackThunder\nStructwrap created by Aleksander && Peter\n");
        if (argc < 2) { printf("Syntax: %s <input.wav> <optional argument if file is in stereo>\n", argv[0]); return -1; }
	char buf[1024], *s = buf;
	strcpy(s, argv[1]);
	char *p = strstr(s, ".wav");
	if (!p) { printf("Please specify a 8KHz, 8-bit wav file!\n"); return -1; }
    

    int is_stereo;
    if (argc > 2) 
    {
        is_stereo = 1; 
    } else 
    {
        is_stereo = 0;
    }
	
	FILE *fi = fopen(s, "rb");
	if (!fi) { printf("Could not open input file!\n"); return -1; }
	strcpy(p, ".h");
	FILE *fo = fopen(s, "wt");
	if (!fo) { printf("Could not open output file!\n"); return -1; }
	printf("lol\n");
	fseek(fi, 0x28, SEEK_SET);
	p = strrchr(s, '\\'); if (p) s = p+1;
	p = strrchr(s, '/'); if (p) s = p+1;
	p = strrchr(s, '.'); if (p) *p = 0;

    /* Create wrapper struct */
    fprintf(fo, "struct song-%s {\n", s);
    
    /* Set stereo */
    fprintf(fo, "\tint is_stereo = %d;\n", is_stereo);

    /* Create note array */
	fprintf(fo, "\tconst unsigned char notes[] =\n\t{");
	int l=0;
	while (1)
	{
		if (l++ % 100 == 0) { fprintf(fo, "\n\t\t"); }
		int a = fgetc(fi); if (a == EOF) break;
		fprintf(fo, "%d,", a);
	}
	fprintf(fo, "\n\t};\n");	

    /* Set array size */
    fprintf(fo, "\tnotes_size = %d;\n", l);

    /* Close struct */
    fprintf(fo, "}\n");

	fclose(fi);
	fclose(fo);
	return 0;
    }
