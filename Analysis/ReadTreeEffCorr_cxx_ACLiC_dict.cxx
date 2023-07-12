// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedImciaccodIKXiCorrelationsdIAnalysisdIReadTreeEffCorr_cxx_ACLiC_dict
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "/home/mciacco/KXiCorrelations/Analysis/./ReadTreeEffCorr.cxx"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *vectorlEMiniXiMCgR_Dictionary();
   static void vectorlEMiniXiMCgR_TClassManip(TClass*);
   static void *new_vectorlEMiniXiMCgR(void *p = nullptr);
   static void *newArray_vectorlEMiniXiMCgR(Long_t size, void *p);
   static void delete_vectorlEMiniXiMCgR(void *p);
   static void deleteArray_vectorlEMiniXiMCgR(void *p);
   static void destruct_vectorlEMiniXiMCgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<MiniXiMC>*)
   {
      vector<MiniXiMC> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<MiniXiMC>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<MiniXiMC>", -2, "vector", 386,
                  typeid(vector<MiniXiMC>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEMiniXiMCgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<MiniXiMC>) );
      instance.SetNew(&new_vectorlEMiniXiMCgR);
      instance.SetNewArray(&newArray_vectorlEMiniXiMCgR);
      instance.SetDelete(&delete_vectorlEMiniXiMCgR);
      instance.SetDeleteArray(&deleteArray_vectorlEMiniXiMCgR);
      instance.SetDestructor(&destruct_vectorlEMiniXiMCgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<MiniXiMC> >()));

      ::ROOT::AddClassAlternate("vector<MiniXiMC>","std::vector<MiniXiMC, std::allocator<MiniXiMC> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<MiniXiMC>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEMiniXiMCgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<MiniXiMC>*)nullptr)->GetClass();
      vectorlEMiniXiMCgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEMiniXiMCgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEMiniXiMCgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<MiniXiMC> : new vector<MiniXiMC>;
   }
   static void *newArray_vectorlEMiniXiMCgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<MiniXiMC>[nElements] : new vector<MiniXiMC>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEMiniXiMCgR(void *p) {
      delete ((vector<MiniXiMC>*)p);
   }
   static void deleteArray_vectorlEMiniXiMCgR(void *p) {
      delete [] ((vector<MiniXiMC>*)p);
   }
   static void destruct_vectorlEMiniXiMCgR(void *p) {
      typedef vector<MiniXiMC> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<MiniXiMC>

namespace ROOT {
   static TClass *vectorlEMiniXigR_Dictionary();
   static void vectorlEMiniXigR_TClassManip(TClass*);
   static void *new_vectorlEMiniXigR(void *p = nullptr);
   static void *newArray_vectorlEMiniXigR(Long_t size, void *p);
   static void delete_vectorlEMiniXigR(void *p);
   static void deleteArray_vectorlEMiniXigR(void *p);
   static void destruct_vectorlEMiniXigR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<MiniXi>*)
   {
      vector<MiniXi> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<MiniXi>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<MiniXi>", -2, "vector", 386,
                  typeid(vector<MiniXi>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEMiniXigR_Dictionary, isa_proxy, 4,
                  sizeof(vector<MiniXi>) );
      instance.SetNew(&new_vectorlEMiniXigR);
      instance.SetNewArray(&newArray_vectorlEMiniXigR);
      instance.SetDelete(&delete_vectorlEMiniXigR);
      instance.SetDeleteArray(&deleteArray_vectorlEMiniXigR);
      instance.SetDestructor(&destruct_vectorlEMiniXigR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<MiniXi> >()));

      ::ROOT::AddClassAlternate("vector<MiniXi>","std::vector<MiniXi, std::allocator<MiniXi> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<MiniXi>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEMiniXigR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<MiniXi>*)nullptr)->GetClass();
      vectorlEMiniXigR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEMiniXigR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEMiniXigR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<MiniXi> : new vector<MiniXi>;
   }
   static void *newArray_vectorlEMiniXigR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<MiniXi>[nElements] : new vector<MiniXi>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEMiniXigR(void *p) {
      delete ((vector<MiniXi>*)p);
   }
   static void deleteArray_vectorlEMiniXigR(void *p) {
      delete [] ((vector<MiniXi>*)p);
   }
   static void destruct_vectorlEMiniXigR(void *p) {
      typedef vector<MiniXi> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<MiniXi>

namespace ROOT {
   static TClass *vectorlEMiniKaonMCgR_Dictionary();
   static void vectorlEMiniKaonMCgR_TClassManip(TClass*);
   static void *new_vectorlEMiniKaonMCgR(void *p = nullptr);
   static void *newArray_vectorlEMiniKaonMCgR(Long_t size, void *p);
   static void delete_vectorlEMiniKaonMCgR(void *p);
   static void deleteArray_vectorlEMiniKaonMCgR(void *p);
   static void destruct_vectorlEMiniKaonMCgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<MiniKaonMC>*)
   {
      vector<MiniKaonMC> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<MiniKaonMC>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<MiniKaonMC>", -2, "vector", 386,
                  typeid(vector<MiniKaonMC>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEMiniKaonMCgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<MiniKaonMC>) );
      instance.SetNew(&new_vectorlEMiniKaonMCgR);
      instance.SetNewArray(&newArray_vectorlEMiniKaonMCgR);
      instance.SetDelete(&delete_vectorlEMiniKaonMCgR);
      instance.SetDeleteArray(&deleteArray_vectorlEMiniKaonMCgR);
      instance.SetDestructor(&destruct_vectorlEMiniKaonMCgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<MiniKaonMC> >()));

      ::ROOT::AddClassAlternate("vector<MiniKaonMC>","std::vector<MiniKaonMC, std::allocator<MiniKaonMC> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<MiniKaonMC>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEMiniKaonMCgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<MiniKaonMC>*)nullptr)->GetClass();
      vectorlEMiniKaonMCgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEMiniKaonMCgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEMiniKaonMCgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<MiniKaonMC> : new vector<MiniKaonMC>;
   }
   static void *newArray_vectorlEMiniKaonMCgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<MiniKaonMC>[nElements] : new vector<MiniKaonMC>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEMiniKaonMCgR(void *p) {
      delete ((vector<MiniKaonMC>*)p);
   }
   static void deleteArray_vectorlEMiniKaonMCgR(void *p) {
      delete [] ((vector<MiniKaonMC>*)p);
   }
   static void destruct_vectorlEMiniKaonMCgR(void *p) {
      typedef vector<MiniKaonMC> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<MiniKaonMC>

namespace ROOT {
   static TClass *vectorlEMiniKaongR_Dictionary();
   static void vectorlEMiniKaongR_TClassManip(TClass*);
   static void *new_vectorlEMiniKaongR(void *p = nullptr);
   static void *newArray_vectorlEMiniKaongR(Long_t size, void *p);
   static void delete_vectorlEMiniKaongR(void *p);
   static void deleteArray_vectorlEMiniKaongR(void *p);
   static void destruct_vectorlEMiniKaongR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<MiniKaon>*)
   {
      vector<MiniKaon> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<MiniKaon>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<MiniKaon>", -2, "vector", 386,
                  typeid(vector<MiniKaon>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEMiniKaongR_Dictionary, isa_proxy, 4,
                  sizeof(vector<MiniKaon>) );
      instance.SetNew(&new_vectorlEMiniKaongR);
      instance.SetNewArray(&newArray_vectorlEMiniKaongR);
      instance.SetDelete(&delete_vectorlEMiniKaongR);
      instance.SetDeleteArray(&deleteArray_vectorlEMiniKaongR);
      instance.SetDestructor(&destruct_vectorlEMiniKaongR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<MiniKaon> >()));

      ::ROOT::AddClassAlternate("vector<MiniKaon>","std::vector<MiniKaon, std::allocator<MiniKaon> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<MiniKaon>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEMiniKaongR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<MiniKaon>*)nullptr)->GetClass();
      vectorlEMiniKaongR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEMiniKaongR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEMiniKaongR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<MiniKaon> : new vector<MiniKaon>;
   }
   static void *newArray_vectorlEMiniKaongR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<MiniKaon>[nElements] : new vector<MiniKaon>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEMiniKaongR(void *p) {
      delete ((vector<MiniKaon>*)p);
   }
   static void deleteArray_vectorlEMiniKaongR(void *p) {
      delete [] ((vector<MiniKaon>*)p);
   }
   static void destruct_vectorlEMiniKaongR(void *p) {
      typedef vector<MiniKaon> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<MiniKaon>

namespace {
  void TriggerDictionaryInitialization_ReadTreeEffCorr_cxx_ACLiC_dict_Impl() {
    static const char* headers[] = {
"./ReadTreeEffCorr.cxx",
nullptr
    };
    static const char* includePaths[] = {
"/var/opt/root-6.26.02/include",
"/home/mciacco/KXiCorrelations/Analysis",
"/home/mciacco/Desktop/Repositories/Utils/include",
"/var/opt/root-6.26.02/etc/",
"/var/opt/root-6.26.02/etc//cling",
"/var/opt/root-6.26.02/etc//cling/plugins/include",
"/var/opt/root-6.26.02/include/",
"/var/opt/root-6.26.02/include",
"/home/mciacco/KXiCorrelations/Analysis",
"/home/mciacco/Desktop/Repositories/Utils/include",
"/var/opt/root-6.26.02/include/",
"/home/mciacco/KXiCorrelations/Analysis/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "ReadTreeEffCorr_cxx_ACLiC_dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
struct __attribute__((annotate("$clingAutoload$../utils/Config_tests.h")))  __attribute__((annotate("$clingAutoload$./ReadTreeEffCorr.cxx")))  MiniKaon;
namespace std{template <typename _Tp> class __attribute__((annotate("$clingAutoload$bits/allocator.h")))  __attribute__((annotate("$clingAutoload$string")))  allocator;
}
struct __attribute__((annotate("$clingAutoload$../utils/Config_tests.h")))  __attribute__((annotate("$clingAutoload$./ReadTreeEffCorr.cxx")))  MiniXi;
struct __attribute__((annotate("$clingAutoload$../utils/Config_tests.h")))  __attribute__((annotate("$clingAutoload$./ReadTreeEffCorr.cxx")))  MiniXiMC;
struct __attribute__((annotate("$clingAutoload$../utils/Config_tests.h")))  __attribute__((annotate("$clingAutoload$./ReadTreeEffCorr.cxx")))  MiniKaonMC;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "ReadTreeEffCorr_cxx_ACLiC_dict dictionary payload"

#ifndef __ACLIC__
  #define __ACLIC__ 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "./ReadTreeEffCorr.cxx"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"ReadTreeEffCorr", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("ReadTreeEffCorr_cxx_ACLiC_dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_ReadTreeEffCorr_cxx_ACLiC_dict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_ReadTreeEffCorr_cxx_ACLiC_dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_ReadTreeEffCorr_cxx_ACLiC_dict() {
  TriggerDictionaryInitialization_ReadTreeEffCorr_cxx_ACLiC_dict_Impl();
}
