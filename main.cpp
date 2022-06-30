#include <iostream>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include "tableau.h"
using namespace std;

int main(int argc, char ** argv) {
  Tableau<const char *> t;
  if(argc < 2) {
    cout << "Aucune image passée en argument!" << endl;
    return 1;
  }
  for(int i=1; i<argc; i++) {
    t.ajouter(argv[i]);
  }
  char *outText;

  tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
  // Initialize tesseract-ocr with English, without specifying tessdata path
  if (api->Init(NULL, "eng")) {
    fprintf(stderr, "Could not initialize tesseract.\n");
    exit(1);
  }

  Pix *image;
  for(int i=0; i<t.taille(); i++) {
    // Open input image with leptonica library
    image = pixRead(t[i]);
    api->SetImage(image);
    // Get OCR result
    outText = api->GetUTF8Text();
    cout << "OCR ouput:\n" << outText <<endl;
  }
  // Destroy used object and release memory
  api->End();
  delete api;
  delete [] outText;
  pixDestroy(&image);

  return 0;
}

