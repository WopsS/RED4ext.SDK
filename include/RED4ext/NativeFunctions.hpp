#include <RED4ext/Scripting/Functions.hpp>
#include <RED4ext/TweakDB.hpp>
using namespace RED4ext;

/// @pattern 48 89 5C 24 ? 57 48 81 EC ? ? ? ? 48 8B 02
/// @nth 1/9
using CBaseFunction_Execute = bool (CBaseFunction* self, CStack* aStack);

/// @pattern 48 89 5C 24 08 57 48 83 EC 20 49 8B D9
/// @nth 0/6
using CGlobalFunction_ctor = CGlobalFunction* (CGlobalFunction*, CName, CName, ScriptingFunction_t<void*>);

/// @pattern 48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 48 8D 05 ? ? ? ? 48 C7 41 08 00 00 00 00
/// @nth 1/3
using CStack_ctor = CStack* (CStack*, ScriptInstance, CStackType*, uint32_t, CStackType*, int64_t);

/// @pattern 48 89 6C 24 20 57 41 54 41 56 48 83 EC ?
/// @nth 0/2
using DynArray_Realloc = void (DynArray<void*> * aThis, uint32_t aCapacity, uint32_t aElementSize, uint32_t aAlignment,
                               void (*a5)(int64_t, int64_t, int64_t, int64_t));

/// @pattern 40 53 48 83 EC 20 33 C0 C6 01 00
/// @nth 2/4
using CString_ctor = CString* (CString*, const char*);

/// @pattern 48 89 5C 24 08 ? 89 ? 24 18 57 48 83 EC 30 8B C2
using TweakDB_CreateRecord = void (TweakDB*, uint32_t aBaseMurmur3, TweakDBID aDBID);

/// @pattern 48 89 5C 24 10 57 48 83 EC 30 F6 41 70 0C 48 8B F9
using CClass_GetMaxAlignment = uint32_t (const CClass*);

/// @pattern 48 89 5C 24 18 56 48 83 EC 20 83 B9 F0 00 00 00 00
using CClass_GetProperty = CProperty* (CClass*, CName);

/// @pattern 40 56 41 56 48 83 EC ? 41 0F B6 F0
using CClass_AllocInstance = ScriptInstance (const CClass*, uint32_t, bool);

/// @pattern 48 89 4C 24 08 53 56 48 83 EC 58
using CClass_sub_D0 = bool (const CClass*);

/// @pattern 40 53 48 83 EC 20 48 83 79 38 00 48 8B D9 75
/// @nth 1/2
using IScriptable_GetValueHolder = void* (IScriptable*);
