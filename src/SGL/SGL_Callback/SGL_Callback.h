#ifndef __SGL_CALLBACK_H__
#define __SGL_CALLBACK_H__

typedef enum SGL_CallbackType {
	SGL_CALLBACK_ONCLICK
} SGL_CallbackType;

typedef void (*SGL_CallbackSignature)(void* parameters);

typedef struct SGL_Callback {
	SGL_CallbackSignature function;
	void* parameters;
} SGL_Callback;

#define SGL_CALLBACK(func, param) \
	(SGL_Callback){func, param}

#endif
