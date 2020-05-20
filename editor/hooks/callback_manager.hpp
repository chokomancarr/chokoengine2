#pragma once
#include "chokoeditor.hpp"
#include "callback_list.hpp"
#include "callback_args.hpp"
#include "callback_sig_args.hpp"
#include "callback_caller.hpp"

CE_BEGIN_ED_NAMESPACE

typedef std::function<void(const ECallbackArgs&, EWindow*)> ECallbackFn;

#define CbFnFrom(fn) [](const ECallbackArgs&, EWindow*) { fn(); }

#define CE_DEF_CALLBACK(nm) static void nm(const ECallbackArgs&, EWindow*)
#define CE_IMPL_CALLBACK(nm) void nm(const ECallbackArgs& args, EWindow* window)

class ECallbackManager {
public:

	static void Init();
	static void Update();

    static void Register(CallbackSig, ECallbackFn);
	static void Invoke(CallbackSig, const ECallbackArgs& args = ECallbackArgs(), EWindow* = nullptr);
};

CE_END_ED_NAMESPACE