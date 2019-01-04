/*
 * JNICALLJAVA.cpp
 *
 *  Created on: 2016/11/14
 *      Author: zhangshanming
 */

#include "JNI_CALL_JAVA_ALGORITHMs.h"

using namespace std;

const string JNI_CALL_JAVA_ALGORITHMs::JAVA_CLASS_PATH =
		"-Djava.class.path=./jalgs/";
const string JNI_CALL_JAVA_ALGORITHMs::JAVA_JAR_FILE_PATH =
		"./jlibs/jgrapht-core-1.0.0.jar";

const string JNI_CALL_JAVA_ALGORITHMs::ALG_K_SHORTESTPATH_CLASSPATH =
		"org/graph/algorithms/shortestpath/kshortestpath/KShortestPath";

const string JNI_CALL_JAVA_ALGORITHMs::ALG_SHORTESTPATH_CLASSPATH =
		"org/graph/algorithms/shortestpath/common/Path";

JNI_CALL_JAVA_ALGORITHMs::JNI_CALL_JAVA_ALGORITHMs() {
	string seprator = ":";
	string wholePath = JNI_CALL_JAVA_ALGORITHMs::JAVA_CLASS_PATH + seprator
			+ JNI_CALL_JAVA_ALGORITHMs::JAVA_JAR_FILE_PATH;
	cout << wholePath << endl;
	char *cstr = new char[wholePath.length() + 1];
	strcpy(cstr, wholePath.c_str());

	this->options[0].optionString = cstr;  // where to find java .class
	this->vm_args.version = JNI_VERSION_1_6;             // minimum Java version
	this->vm_args.nOptions = 1;                          // number of options
	this->vm_args.options = this->options;
	this->vm_args.ignoreUnrecognized = false; // invalid options make the JVM init fail

	//=============== load and initialize Java VM and JNI interface =============
	jint rc = JNI_CreateJavaVM(&this->jvm, (void**) &this->env, &this->vm_args); // YES !!
	if (rc != JNI_OK) {
		// TO DO: error processing...
		cerr << "ERROR: Load and Initialize Java VM and JNI Failure !!!"
				<< endl;
		exit(EXIT_FAILURE);
	}
	this->kshortestPath = NULL;
	this->kshortestPath_instance = NULL;
}

JNI_CALL_JAVA_ALGORITHMs::~JNI_CALL_JAVA_ALGORITHMs() {
	this->jvm->DestroyJavaVM();
}

void JNI_CALL_JAVA_ALGORITHMs::JNI_CALL_JAVA_Constructor_KShortestPath_Instance(
		const string fileName) {
	const char * class_file =
			JNI_CALL_JAVA_ALGORITHMs::ALG_K_SHORTESTPATH_CLASSPATH.c_str();
	this->kshortestPath = this->env->FindClass(class_file);
	if (this->kshortestPath == nullptr) {
		cout << class_file << endl;
		cerr << "ERROR: KShortestPath class not found !";
		exit(EXIT_FAILURE);
	}

	jmethodID constructor = this->env->GetMethodID(this->kshortestPath,
			"<init>", "(Ljava/lang/String;)V");
	if (constructor == nullptr) {
		cerr << "ERROR: The constructor method of KShortestPath  is not found !"
				<< endl;
		exit(EXIT_FAILURE);
	}

	jstring fname = this->env->NewStringUTF(fileName.c_str());
	this->kshortestPath_instance = this->env->NewObject(this->kshortestPath,
			constructor, fname);

	if (this->kshortestPath_instance == 0) {
		cerr << "ERROR: The instance initialization of KShortestPath  failure !"
				<< endl;
		exit(EXIT_FAILURE);
	}

}

void JNI_CALL_JAVA_ALGORITHMs::JNI_CALL_JAVA_Get_KShortestPaths(
		const string source, const string destination) {
	cout << " Funcation: JNI_CALL_JAVA_Get_KShortestPaths perform Start"
			<< endl;
	jstring src = this->env->NewStringUTF(source.c_str());
	jstring dest = this->env->NewStringUTF(destination.c_str());

	jclass java_util_ArrayList = this->env->FindClass("java/util/ArrayList");
	jmethodID java_util_ArrayList_size = this->env->GetMethodID(
			java_util_ArrayList, "size", "()I");
	jmethodID java_util_ArrayList_get = this->env->GetMethodID(
			java_util_ArrayList, "get", "(I)Ljava/lang/Object;");

	jclass path_class = this->env->FindClass(
			JNI_CALL_JAVA_ALGORITHMs::ALG_SHORTESTPATH_CLASSPATH.c_str());
	jmethodID get_path_length = this->env->GetMethodID(path_class, "getLength",
			"()I");
	jmethodID get_node_info = this->env->GetMethodID(path_class, "getNodeInfo",
			"()Ljava/util/ArrayList;");
	jmethodID get_path_cost = this->env->GetMethodID(path_class, "getCost",
			"()D");

	jmethodID getKsPaths = this->env->GetMethodID(this->kshortestPath,
			"getKShortestPath",
			"(Ljava/lang/String;Ljava/lang/String;)Ljava/util/ArrayList;");
	if (getKsPaths == nullptr) {
		cerr << "ERROR: The getKShortestPath of KShortestPath  not found !"
				<< endl;
		exit(EXIT_FAILURE);
	}
	jobject resultPaths = this->env->CallObjectMethod(
			this->kshortestPath_instance, getKsPaths, src, dest);

	jint pSize = this->env->CallIntMethod(resultPaths,
			java_util_ArrayList_size);
	for (jint pIndex = 0; pIndex < pSize; ++pIndex) {
		jobject aPath = this->env->CallObjectMethod(resultPaths,
				java_util_ArrayList_get, pIndex);
		jobject nodes = this->env->CallObjectMethod(aPath, get_node_info);
		jint nodeSize = this->env->CallIntMethod(nodes,
				java_util_ArrayList_size);
		for (jint nIndex = 0; nIndex < nodeSize; ++nIndex) {
			jstring aNode = (jstring) this->env->CallObjectMethod(nodes,
					java_util_ArrayList_get, nIndex);
		}

		jint length = this->env->CallIntMethod(aPath, get_path_length);
		jdouble cost = this->env->CallDoubleMethod(aPath, get_path_cost);
	}

	cout << " Funcation: JNI_CALL_JAVA_Get_KShortestPaths perform End" << endl;
}

void JNI_CALL_JAVA_ALGORITHMs::JNI_CALL_JAVA_Get_KShortestPathsWith_KLimit(
		const string source, const string destination, const int k) {
	cout << " Funcation: JNI_CALL_JAVA_Get_KShortestPathsWith_KLimit perform Start"
			<< endl;
	jstring src = this->env->NewStringUTF(source.c_str());
	jstring dest = this->env->NewStringUTF(destination.c_str());
	jint kLimit = (jint) k;
	jclass java_util_ArrayList = this->env->FindClass("java/util/ArrayList");
	jmethodID java_util_ArrayList_size = this->env->GetMethodID(
			java_util_ArrayList, "size", "()I");
	jmethodID java_util_ArrayList_get = this->env->GetMethodID(
			java_util_ArrayList, "get", "(I)Ljava/lang/Object;");

	jclass path_class = this->env->FindClass(
			JNI_CALL_JAVA_ALGORITHMs::ALG_SHORTESTPATH_CLASSPATH.c_str());
	jmethodID get_path_length = this->env->GetMethodID(path_class, "getLength",
			"()I");
	jmethodID get_node_info = this->env->GetMethodID(path_class, "getNodeInfo",
			"()Ljava/util/ArrayList;");
	jmethodID get_path_cost = this->env->GetMethodID(path_class, "getCost",
			"()D");

	jmethodID getKsPaths = this->env->GetMethodID(this->kshortestPath,
			"getKShortestPathWithKLimit",
			"(Ljava/lang/String;Ljava/lang/String;I)Ljava/util/ArrayList;");
	if (getKsPaths == nullptr) {
		cerr << "ERROR: The getKShortestPath of KShortestPath  not found !"
				<< endl;
		exit(EXIT_FAILURE);
	}
	jobject resultPaths = this->env->CallObjectMethod(
			this->kshortestPath_instance, getKsPaths, src, dest, kLimit);

	jint pSize = this->env->CallIntMethod(resultPaths,
			java_util_ArrayList_size);
	for (jint pIndex = 0; pIndex < pSize; ++pIndex) {
		jobject aPath = this->env->CallObjectMethod(resultPaths,
				java_util_ArrayList_get, pIndex);
		jobject nodes = this->env->CallObjectMethod(aPath, get_node_info);
		jint nodeSize = this->env->CallIntMethod(nodes,
				java_util_ArrayList_size);
		for (jint nIndex = 0; nIndex < nodeSize; ++nIndex) {
			jstring aNode = (jstring) this->env->CallObjectMethod(nodes,
					java_util_ArrayList_get, nIndex);
		}

		jint length = this->env->CallIntMethod(aPath, get_path_length);
		jdouble cost = this->env->CallDoubleMethod(aPath, get_path_cost);
	}

	cout << " Funcation: JNI_CALL_JAVA_Get_KShortestPathsWith_KLimit perform End" << endl;
}

void JNI_CALL_JAVA_ALGORITHMs::JNI_CALL_JAVA_Get_KShortestPathsWith_HopMinimumLimit(
		const string source, const string destination, const int minimumHop) {
	cout << " Funcation: JNI_CALL_JAVA_Get_KShortestPathsWith_HopMinimumLimit perform Start"
				<< endl;
		jstring src = this->env->NewStringUTF(source.c_str());
		jstring dest = this->env->NewStringUTF(destination.c_str());
		jint minHopLimit = (jint) minimumHop;

		jclass java_util_ArrayList = this->env->FindClass("java/util/ArrayList");
		jmethodID java_util_ArrayList_size = this->env->GetMethodID(
				java_util_ArrayList, "size", "()I");
		jmethodID java_util_ArrayList_get = this->env->GetMethodID(
				java_util_ArrayList, "get", "(I)Ljava/lang/Object;");

		jclass path_class = this->env->FindClass(
				JNI_CALL_JAVA_ALGORITHMs::ALG_SHORTESTPATH_CLASSPATH.c_str());
		jmethodID get_path_length = this->env->GetMethodID(path_class, "getLength",
				"()I");
		jmethodID get_node_info = this->env->GetMethodID(path_class, "getNodeInfo",
				"()Ljava/util/ArrayList;");
		jmethodID get_path_cost = this->env->GetMethodID(path_class, "getCost",
				"()D");

		jmethodID getKsPaths = this->env->GetMethodID(this->kshortestPath,
				"getKShortestPathWithHopMinimumLimit",
				"(Ljava/lang/String;Ljava/lang/String;I)Ljava/util/ArrayList;");
		if (getKsPaths == nullptr) {
			cerr << "ERROR: The getKShortestPath of KShortestPath  not found !"
					<< endl;
			exit(EXIT_FAILURE);
		}
		jobject resultPaths = this->env->CallObjectMethod(
				this->kshortestPath_instance, getKsPaths, src, dest, minHopLimit);
cout << "--------" << endl;
		jint pSize = this->env->CallIntMethod(resultPaths,
				java_util_ArrayList_size);
		for (jint pIndex = 0; pIndex < pSize; ++pIndex) {
			jobject aPath = this->env->CallObjectMethod(resultPaths,
					java_util_ArrayList_get, pIndex);
			jobject nodes = this->env->CallObjectMethod(aPath, get_node_info);
			jint nodeSize = this->env->CallIntMethod(nodes,
					java_util_ArrayList_size);
			cout << "nodes : " << endl;
			for (jint nIndex = 0; nIndex < nodeSize; ++nIndex) {
				jstring aNode = (jstring) this->env->CallObjectMethod(nodes,
						java_util_ArrayList_get, nIndex);
				cout << aNode << " ";
			}

			jint length = this->env->CallIntMethod(aPath, get_path_length);
			jdouble cost = this->env->CallDoubleMethod(aPath, get_path_cost);
			cout << ";  length :" << length << "; cost: " << cost << endl;
		}

		cout << " Funcation: JNI_CALL_JAVA_Get_KShortestPathsWith_HopMinimumLimit perform End" << endl;
}

void JNI_CALL_JAVA_ALGORITHMs::JNI_CALL_JAVA_Get_KShortestPathsWith_HopMaximumLimit(
		const string source, const string destination, const int maximumHop) {

	cout << " Funcation: JNI_CALL_JAVA_Get_KShortestPathsWith_HopMaximumLimit perform Start"
				<< endl;
		jstring src = this->env->NewStringUTF(source.c_str());
		jstring dest = this->env->NewStringUTF(destination.c_str());
		jint maxHopLimit = (jint) maximumHop;

		jclass java_util_ArrayList = this->env->FindClass("java/util/ArrayList");
		jmethodID java_util_ArrayList_size = this->env->GetMethodID(
				java_util_ArrayList, "size", "()I");
		jmethodID java_util_ArrayList_get = this->env->GetMethodID(
				java_util_ArrayList, "get", "(I)Ljava/lang/Object;");

		jclass path_class = this->env->FindClass(
				JNI_CALL_JAVA_ALGORITHMs::ALG_SHORTESTPATH_CLASSPATH.c_str());
		jmethodID get_path_length = this->env->GetMethodID(path_class, "getLength",
				"()I");
		jmethodID get_node_info = this->env->GetMethodID(path_class, "getNodeInfo",
				"()Ljava/util/ArrayList;");
		jmethodID get_path_cost = this->env->GetMethodID(path_class, "getCost",
				"()D");

		jmethodID getKsPaths = this->env->GetMethodID(this->kshortestPath,
				"getKShortestPathWithHopMaximumLimit",
				"(Ljava/lang/String;Ljava/lang/String;I)Ljava/util/ArrayList;");
		if (getKsPaths == nullptr) {
			cerr << "ERROR: The getKShortestPath of KShortestPath  not found !"
					<< endl;
			exit(EXIT_FAILURE);
		}
		jobject resultPaths = this->env->CallObjectMethod(
				this->kshortestPath_instance, getKsPaths, src, dest, maxHopLimit);
cout << "--------" << endl;
		jint pSize = this->env->CallIntMethod(resultPaths,
				java_util_ArrayList_size);
		for (jint pIndex = 0; pIndex < pSize; ++pIndex) {
			jobject aPath = this->env->CallObjectMethod(resultPaths,
					java_util_ArrayList_get, pIndex);
			jobject nodes = this->env->CallObjectMethod(aPath, get_node_info);
			jint nodeSize = this->env->CallIntMethod(nodes,
					java_util_ArrayList_size);
			cout << "nodes : " << endl;
			for (jint nIndex = 0; nIndex < nodeSize; ++nIndex) {
				jstring aNode = (jstring) this->env->CallObjectMethod(nodes,
						java_util_ArrayList_get, nIndex);
				cout << aNode << " ";
			}

			jint length = this->env->CallIntMethod(aPath, get_path_length);
			jdouble cost = this->env->CallDoubleMethod(aPath, get_path_cost);
			cout << ";  length :" << length << "; cost: " << cost << endl;
		}

		cout << " Funcation: JNI_CALL_JAVA_Get_KShortestPathsWith_HopMaximumLimit perform End" << endl;
}

void JNI_CALL_JAVA_ALGORITHMs::JNI_CALL_JAVA_Get_KShortestPathsWith_HopRangeLimit(
		const string source, const string destination, const int minimumHop,
		const int maximumHop) {

	cout << " Funcation: JNI_CALL_JAVA_Get_KShortestPathsWith_HopRangeLimit perform Start"
				<< endl;
		jstring src = this->env->NewStringUTF(source.c_str());
		jstring dest = this->env->NewStringUTF(destination.c_str());
		jint maxHopLimit = (jint) maximumHop;
		jint minHopLimit = (jint) minimumHop;

		jclass java_util_ArrayList = this->env->FindClass("java/util/ArrayList");
		jmethodID java_util_ArrayList_size = this->env->GetMethodID(
				java_util_ArrayList, "size", "()I");
		jmethodID java_util_ArrayList_get = this->env->GetMethodID(
				java_util_ArrayList, "get", "(I)Ljava/lang/Object;");

		jclass path_class = this->env->FindClass(
				JNI_CALL_JAVA_ALGORITHMs::ALG_SHORTESTPATH_CLASSPATH.c_str());
		jmethodID get_path_length = this->env->GetMethodID(path_class, "getLength",
				"()I");
		jmethodID get_node_info = this->env->GetMethodID(path_class, "getNodeInfo",
				"()Ljava/util/ArrayList;");
		jmethodID get_path_cost = this->env->GetMethodID(path_class, "getCost",
				"()D");

		jmethodID getKsPaths = this->env->GetMethodID(this->kshortestPath,
				"getKShortestPathWithHopRangeLimit",
				"(Ljava/lang/String;Ljava/lang/String;II)Ljava/util/ArrayList;");
		if (getKsPaths == nullptr) {
			cerr << "ERROR: The getKShortestPath of KShortestPath  not found !"
					<< endl;
			exit(EXIT_FAILURE);
		}
		jobject resultPaths = this->env->CallObjectMethod(
				this->kshortestPath_instance, getKsPaths, src, dest, minHopLimit, maxHopLimit);
cout << "--------" << endl;
		jint pSize = this->env->CallIntMethod(resultPaths,
				java_util_ArrayList_size);
		for (jint pIndex = 0; pIndex < pSize; ++pIndex) {
			jobject aPath = this->env->CallObjectMethod(resultPaths,
					java_util_ArrayList_get, pIndex);
			jobject nodes = this->env->CallObjectMethod(aPath, get_node_info);
			jint nodeSize = this->env->CallIntMethod(nodes,
					java_util_ArrayList_size);
			cout << "nodes : " << endl;
			for (jint nIndex = 0; nIndex < nodeSize; ++nIndex) {
				jstring aNode = (jstring) this->env->CallObjectMethod(nodes,
						java_util_ArrayList_get, nIndex);
				cout << aNode << " ";
			}

			jint length = this->env->CallIntMethod(aPath, get_path_length);
			jdouble cost = this->env->CallDoubleMethod(aPath, get_path_cost);
			cout << ";  length :" << length << "; cost: " << cost << endl;
	}

		cout << " Funcation: JNI_CALL_JAVA_Get_KShortestPathsWith_HopRangeLimit perform End" << endl;
}

string JNI_CALL_JAVA_ALGORITHMs::Convert_JString_to_STDString(jstring jstr) {
	const char* cstr = this->env->GetStringUTFChars(jstr, 0);
	char *str = strdup(cstr);
	this->env->ReleaseStringUTFChars(jstr, cstr);

	stringstream sstr;
	sstr << str;
	return sstr.str();
}
