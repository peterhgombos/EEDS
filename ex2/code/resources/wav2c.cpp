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
	fseek(fi, 0x28, SEEK_SET);
	p = strrchr(s, '\\'); if (p) s = p+1;
	p = strrchr(s, '/'); if (p) s = p+1;
	p = strrchr(s, '.'); if (p) *p = 0;

  /* Import song header */
  fprintf(fo, "#include \"../../songs.h\"\n\n");

  /* Create note array */
	fprintf(fo, "const uint8_t notes_%s[] =\n{", s);
	int l=0;
	while (1)
	{
		if (l++ % 100 == 0) { fprintf(fo, "\n\t"); }
		int a = fgetc(fi); if (a == EOF) break;
		fprintf(fo, "%d,", a);
	}
	fprintf(fo, "\n};\n\n");	
  
  /* Create struct */
  fprintf(fo, "song_t %s = {\n", s);

  /* Set stereo */
  fprintf(fo, "\t.is_stereo = %d,\n", is_stereo);

  /* Set notes */
  fprintf(fo, "\t.notes = notes_%s,\n", s);

  /* Set notes size */
  fprintf(fo, "\t.notes_size = %d\n", l);

  /* Close struct */
  fprintf(fo, "};\n");

	fclose(fi);
	fclose(fo);
	return 0;
}
