//void doReadPico(const int ev, const Char_t* inputlist = "???", const Char_t* ouputfile)
//void doReadPico(const Char_t* inputlist = "???", const Char_t* ouputfile)
void doReadPico(const Char_t* inputlist = "???", const Char_t* outputfile, const Char_t* outpi, const Char_t* outka, const Char_t* outpr)
{
  gSystem->Load("ReadPico");
  ReadPico t;
  //t.LoopAll(inputlist, ouputfile);
  t.LoopAll(inputlist, outputfile, outpi, outka, outpr);
}
