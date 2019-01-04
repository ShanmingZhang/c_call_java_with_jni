//============================================================================
// Name        : calljavawithjni.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
//javap -s -p jp/ac/keio/ics/yamanaka/graph/kshortestpath/KShortestPath.class
/* Remember to include this */

#include <jni.h>
#include <jni_md.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

using namespace std;

JNIEnv * JNI_InitializeJVMInterface(char* class_directory, string jar_path) {
	JavaVM *jvm;                    // Pointer to the JVM (Java Virtual Machine)
	JNIEnv *env;                      // Pointer to native interface
	//================== prepare loading of Java VM ============================
	JavaVMInitArgs vm_args;                        // Initialization arguments
	JavaVMOption options[1]; // A list of options to build a JVM from C++
	//char * optStr = "-Djava.class.path=" + class_directory.c_str();
	options[0].optionString = class_directory;
	// where to find java .class
	vm_args.version = JNI_VERSION_1_6;             // minimum Java version
	vm_args.nOptions = 1;                          // number of options
	vm_args.options = options;
	vm_args.ignoreUnrecognized = false; // invalid options make the JVM init fail
	//=============== load and initialize Java VM and JNI interface =============
	jint rc = JNI_CreateJavaVM(&jvm, (void**) &env, &vm_args);  // YES !!
	if (rc != JNI_OK) {
		// TO DO: error processing...
		cout << " JNI call failure " << endl;
		exit(EXIT_FAILURE);
	}

	return env;
}

void JNI_CallJavaNormalMethodByJavaObject(JNIEnv *env, string class_path) {
	jclass cls2 = env->FindClass(class_path.c_str()); // try to find the class
	if (cls2 == nullptr) {
		cout << class_path.c_str() << endl;
		cerr << "ERROR: class not found !";
	} else {                                  // if class found, continue
		cout << "Class MyTest found" << endl;
		jmethodID mid2 = env->GetMethodID(cls2, "<init>", "()V");
		cout << "Class MyTest found" << endl;
		if (mid2 == nullptr) {
			cerr << "ERROR: method it main2(int) not found !" << endl;
		} else {
			jobject instance = env->NewObject(cls2, mid2);
			cout << " The OK " << endl;
			if (instance != 0) {
				jmethodID show = env->GetMethodID(cls2, "showId", "()V");
				if (show == nullptr)
					cerr << "No showId method !!" << endl;
				else {

					cout << " The OK " << endl;
					env->CallVoidMethod(instance, show);
				}
			} else {

				cout << " The failure " << endl;
			}

			//cout << " The result " << resInt << endl;
			cout << endl;
		}
	}
}

void JNI_CallJavaMethodReturnArrayList(JNIEnv *env, string class_path) {
	jclass java_util_ArrayList;
	jmethodID java_util_ArrayList_init;
	jmethodID java_util_ArrayList_size;
	jmethodID java_util_ArrayList_get;
	jmethodID java_util_ArrayList_add;

	java_util_ArrayList = env->NewGlobalRef(
			env->FindClass("java/util/ArrayList"));
	java_util_ArrayList_init = env->GetMethodID(java_util_ArrayList, "<init>",
			"(I)V");
	java_util_ArrayList_size = env->GetMethodID(java_util_ArrayList, "size",
			"()I");
	java_util_ArrayList_get = env->GetMethodID(java_util_ArrayList, "get",
			"(I)Ljava/lang/Object;");
	java_util_ArrayList_add = env->GetMethodID(java_util_ArrayList, "add",
			"(Ljava/lang/Object;)Z");

//	std::vector<std::string> java2cpp(jobject arrayList) {
//		jint len = env->CallIntMethod(arrayList, java_util_ArrayList_size);
//		std::vector<std::string> result;
//		result.reserve(len);
//		for (jint i=0; i<len; i++) {
//			jstring element = static_cast<jstring>(env->CallObjectMethod(arrayList, java_util_ArrayList_get, i));
//			const char* pchars = env->GetStringUTFChars(element, nullptr);
//			result.emplace_back(pchars);
//			env->ReleaseStringUTFChars(element, pchars);
//			env->DeleteLocalRef(element);
//		}
//	}

	jclass cls2 = env->FindClass(class_path.c_str()); // try to find the class
	jclass jarraylist = env->FindClass("java/util/ArrayList"); // try to find the class
	if (cls2 == nullptr) {
		cout << class_path.c_str() << endl;
		cerr << "ERROR: class not found !";
	} else {                                  // if class found, continue
		cout << "Class MyTest found" << endl;
		jmethodID mid2 = env->GetMethodID(cls2, "<init>", "()V");

		//jmethodID arrayconstructor = env->GetMethodID(jarraylist, "<init>",
		//	"()V");
		cout << "Class MyTest found" << endl;
		if (mid2 == nullptr) {
			cerr << "ERROR: method it main2(int) not found !" << endl;
		} else {
			jobject instance = env->NewObject(cls2, mid2);
//			jobject arrayinstance = env->NewObject(jarraylist,
//					arrayconstructor);
			cout << " The OK " << endl;
			if (instance != 0) {
				jmethodID show = env->GetMethodID(cls2, "getList",
						"()Ljava/util/ArrayList;");
				if (show == nullptr)
					cerr << "No showId method !!" << endl;
				else {

					//cout << " The OK " << arrayinstance << endl;
					//env->CallVoidMethod(simpleJNITestInstance, show);

					jobject arrayinstance = (jobject) env->CallObjectMethod(
							instance, show);

					cout << " The result "
							<< env->CallIntMethod(arrayinstance,
									java_util_ArrayList_size) << endl;
					jstring jstr;

					jmethodID jmthd = env->GetMethodID(jarraylist, "get",
							"(I)Ljava/lang/Object");
					cout << " OK " << endl;
					jint i = 0;
					jstr = (jstring) env->CallObjectMethod(arrayinstance,
							java_util_ArrayList_get, i);
					const char* cstr = env->GetStringUTFChars(jstr, 0);
					char *str = strdup(cstr);
					env->ReleaseStringUTFChars(jstr, cstr);

					stringstream sstr;
					sstr << str;
					string myString = sstr.str();
					cout << " The result " << myString << endl;
				}
			} else {

				cout << " The failure " << endl;
			}

			cout << endl;
		}
	}
}

void JNI_CallJavaMethodReturnString(JNIEnv *env, string class_path) {
	jclass cls2 = env->FindClass(class_path.c_str()); // try to find the class
	if (cls2 == nullptr) {
		cout << class_path.c_str() << endl;
		cerr << "ERROR: class not found !";
	} else {                                  // if class found, continue
		cout << "Class MyTest found" << endl;
		jmethodID mid2 = env->GetMethodID(cls2, "<init>", "()V");
		cout << "Class MyTest found" << endl;
		if (mid2 == nullptr) {
			cerr << "ERROR: method it main2(int) not found !" << endl;
		} else {
			jobject instance = env->NewObject(cls2, mid2);
			cout << " The OK " << endl;
			if (instance != 0) {
				jmethodID show = env->GetMethodID(cls2, "getString",
						"()Ljava/lang/String;");
				if (show == nullptr)
					cerr << "No showId method !!" << endl;
				else {

					cout << " The OK " << endl;
					//env->CallVoidMethod(simpleJNITestInstance, show);
					jstring jstr;
					jstr = (jstring) env->CallObjectMethod(instance, show);
					const char* cstr = env->GetStringUTFChars(jstr, 0);
					char *str = strdup(cstr);
					env->ReleaseStringUTFChars(jstr, cstr);

					stringstream sstr;
					sstr << str;
					string myString = sstr.str();
					cout << " The result " << myString << endl;
				}
			} else {

				cout << " The failure " << endl;
			}

			cout << endl;
		}
	}
}

void JNI_CallJavaStaticMethodWithArray(JNIEnv *env, string class_path) {
	jclass cls2 = env->FindClass(class_path.c_str()); // try to find the class
	if (cls2 == nullptr) {
		cout << class_path.c_str() << endl;
		cerr << "ERROR: class not found !";
	} else {                                  // if class found, continue
		cout << "Class MyTest found" << endl;
		jmethodID mid2 = env->GetStaticMethodID(cls2, "main",
				"([Ljava/lang/String;)V");
		if (mid2 == nullptr) {
			cerr << "ERROR: method it main2(int) not found !" << endl;
		} else {
			jobjectArray arr = env->NewObjectArray(2, // constructs java array of 5
					env->FindClass("java/lang/String"),    // Strings
					env->NewStringUTF("str")); // each initialized with value "str"
			env->SetObjectArrayElement(arr, 0,
					env->NewStringUTF("From C++ String"));  // change an element
			env->CallStaticVoidMethod(cls2, mid2, arr); // call the method with the arr as argument.
			env->DeleteLocalRef(arr);
			//cout << " The result " << resInt << endl;
			cout << endl;
		}
	}
}

void JNI_CallJavaStaticMethodWithInputandOutput(JNIEnv *env,
		string class_path) {

	jclass cls2 = env->FindClass(class_path.c_str()); // try to find the class
	if (cls2 == nullptr) {
		cout << class_path.c_str() << endl;
		cerr << "ERROR: class not found !";
	} else {                                  // if class found, continue
		cout << "Class MyTest found" << endl;
		jmethodID mid2 = env->GetStaticMethodID(cls2, "mymain2", "(I)I");
		if (mid2 == nullptr) {
			cerr << "ERROR: method it main2(int) not found !" << endl;
		} else {
			jint resInt = env->CallStaticIntMethod(cls2, mid2, (jint) 5);
			cout << " The result " << resInt << endl;
			cout << endl;
		}
	}
}

void JNI_CallJavaStaticMethodWithNonInputandOutput(JNIEnv *env,
		string class_path) {

	jclass cls2 = env->FindClass(class_path.c_str()); // try to find the class
	if (cls2 == nullptr) {
		cerr << "ERROR: class not found !";
	} else {                                  // if class found, continue
		cout << "Class MyTest found" << endl;
		jmethodID mid = env->GetStaticMethodID(cls2, "mymain", "()V"); // find method
		if (mid == nullptr)
			cerr << "ERROR: method void mymain() not found !" << endl;
		else {
			env->CallStaticVoidMethod(cls2, mid);             // call method
			cout << endl;
		}
	}
}

/* This is a simple main file to show how to call Java from C++ */

int main() {
	string path_base = "-Djava.class.path=";
	string path_directory =
			"/Users/zhangshanming/eclipse/java-neon/workspace/SimpleJNITest/bin/";
	string class_path = "com/example/simple/MyTest";

	cout << " -------------------  " << endl;
	string path = path_base + path_directory;
	char *cstr = new char[path.length() + 1];
	strcpy(cstr, path.c_str());

	JNIEnv *env = JNI_InitializeJVMInterface(cstr, "");
//JNI_CallJavaStaticMethodWithNonInputandOutput(env, class_path);

//JNI_CallJavaStaticMethodWithInputandOutput(env, class_path);

//JNI_CallJavaStaticMethodWithArray(env, class_path);

//JNI_CallJavaNormalMethodByJavaObject(env, class_path);
//JNI_CallJavaMethodReturnString(env, class_path);
	JNI_CallJavaMethodReturnArrayList(env, class_path);
	delete[] cstr;
	printf("All done, bye bye!\n");
	return 0;

}
