#ifndef SGL_TEXT
#define SGL_TEXT

typedef struct SGL_Text {
	const char* content;
} SGL_Text;

SGL_Text* SGL_TextNew(const char* text);
void SGL_TextDestroy(SGL_Text* target);

#endif
