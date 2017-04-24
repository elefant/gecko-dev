#include "PBrowserFuzzer.h"
#include "nsPrintfCString.h"

#include "FuzzTraitsPrimitives.h"
#include "FuzzTraitsCommon.h"
#include "FuzzTraitsPBrowser.h"

#undef LOG
#define LOG(args) printf_stderr(">>>>>> PBrowserFuzzer <<<<<<< %s\n", (nsPrintfCString args).get())
#include "FuzzyCall.h"

namespace mozilla {

PBrowserFuzzer::PBrowserFuzzer(dom::PBrowserChild* aPBrowserChild)
  : mPBrowserChild(aPBrowserChild)
{
}

nsresult
PBrowserFuzzer::CreateParentActor()
{
  return mPBrowserChild ? NS_OK : NS_ERROR_FAILURE;
}

bool
PBrowserFuzzer::SendOneIPCMessage()
{
  const uint32_t kParentMessageNum = 70;

  int callIndex = mCallIndex++ % kParentMessageNum;
  switch (callIndex) {
  // case 0: return FUZZY_CALL4(AsyncMessage, nsString, CpowEntry[], Principal, ClonedMessageData);
  //case 1: return FUZZY_CALL0(mPBrowserChild, PRenderFrameConstructor);
  //case 2: return FUZZY_CALL4(mPBrowserChild, PDocAccessible, nullable PDocAccessible, uint64_t, uint32_t, IAccessibleHolder);
  //case 3: return FUZZY_CALL0(mPBrowserChild, PPluginWidgetConstructor);
  //case 4: return FUZZY_CALL1(mPBrowserChild, GetWidgetNativeData, WindowsHandle&);
  //case 5: return FUZZY_CALL1(mPBrowserChild, SetNativeChildOfShareableWindow, uintptr_t);
  case 6: return FUZZY_CALL0(mPBrowserChild, DispatchFocusToTopLevelWindow);
  case 7: return FUZZY_CALL2(mPBrowserChild, MoveFocus, bool, bool);
  case 8: return FUZZY_CALL5(mPBrowserChild, SizeShellTo, uint32_t, int32_t, int32_t, int32_t, int32_t);
  //case 9: return FUZZY_CALL1(mPBrowserChild, DropLinks, nsString[]);
  //case 10: return FUZZY_CALL1(mPBrowserChild, Event, RemoteDOMEvent);
  //case 11: return FUZZY_CALL1(mPBrowserChild, SyncMessage, nsString, ClonedMessageData, CpowEntry[], Principal, &StructuredCloneData[]);
  //case 12: return FUZZY_CALL3(mPBrowserChild, NotifyIMEFocus, ContentCache, IMENotification, &nsIMEUpdatePreference);
  case 13: return FUZZY_CALL2(mPBrowserChild, NotifyIMETextChange, ContentCache, IMENotification);
  case 14: return FUZZY_CALL2(mPBrowserChild, NotifyIMECompositionUpdate, ContentCache, IMENotification);
  case 15: return FUZZY_CALL2(mPBrowserChild, NotifyIMESelection, ContentCache, IMENotification);
  case 16: return FUZZY_CALL1(mPBrowserChild, UpdateContentCache, ContentCache);
  case 17: return FUZZY_CALL2(mPBrowserChild, NotifyIMEMouseButtonEvent, IMENotification, bool&);
  case 18: return FUZZY_CALL2(mPBrowserChild, NotifyIMEPositionChange, ContentCache, IMENotification);
  case 19: return FUZZY_CALL3(mPBrowserChild, RequestIMEToCommitComposition, bool, bool&, nsString&);
  //case 20: return FUZZY_CALL2(mPBrowserChild, OnEventNeedingAckHandled, EventMessage);
  //case 21: return FUZZY_CALL2(mPBrowserChild, StartPluginIME, WidgetKeyboardEvent, int32_t, int32_t, nsString&);
  case 22: return FUZZY_CALL1(mPBrowserChild, SetPluginFocused, bool);
  //case 23: return FUZZY_CALL2(mPBrowserChild, SetCandidateWindowForPlugin, CandidateWindowPosition);
  //case 24: return FUZZY_CALL2(mPBrowserChild, OnWindowedPluginKeyEvent, NativeEventData);
  //case 25: return FUZZY_CALL2(mPBrowserChild, DefaultProcOfPluginEvent, WidgetPluginEvent);
  case 26: return FUZZY_CALL1(mPBrowserChild, RequestFocus, bool);
  //case 27: return FUZZY_CALL2(mPBrowserChild, EnableDisableCommands, nsString, nsCString[], nsCString[]);
  //case 28: return FUZZY_CALL2(mPBrowserChild, GetInputContext, IMEEnabled&, &IMEOpen);
  case 29: return FUZZY_CALL7(mPBrowserChild, SetInputContext, int32_t, int32_t, nsString, nsString, nsString, int32_t, int32_t);
  case 30: return FUZZY_CALL1(mPBrowserChild, GetDPI, float&);
  case 31: return FUZZY_CALL1(mPBrowserChild, GetDefaultScale, double&);
  case 32: return FUZZY_CALL1(mPBrowserChild, GetWidgetRounding, int32_t&);
  case 33: return FUZZY_CALL1(mPBrowserChild, GetMaxTouchPoints, uint32_t&);
  case 34: return FUZZY_CALL2(mPBrowserChild, SetCursor, uint32_t, bool);
  case 35: return FUZZY_CALL8(mPBrowserChild, SetCustomCursor, nsCString, uint32_t, uint32_t, uint32_t, uint8_t, uint32_t, uint32_t, bool);
  case 36: return FUZZY_CALL2(mPBrowserChild, SetStatus, uint32_t, nsString);
  case 37: return FUZZY_CALL4(mPBrowserChild, ShowTooltip, uint32_t, uint32_t, nsString, nsString);
  case 38: return FUZZY_CALL0(mPBrowserChild, HideTooltip);
  //case 39: return FUZZY_CALL2(mPBrowserChild, PColorPickerConstructor, nsString, nsString);
  //case 40: return FUZZY_CALL2(mPBrowserChild, PFilePicker, nsString, int16_t);
  //case 41: return FUZZY_CALL2(mPBrowserChild, PIndexedDBPermissionRequestConstructor, Principal);
  //case 42: return FUZZY_CALL2(mPBrowserChild, BrowserFrameOpenWindow, PBrowser, PRenderFrame, nsString, nsString, bool&, TextureFactoryIdentifier&, uint64_t&);
  case 43: return FUZZY_CALL2(mPBrowserChild, RespondStartSwipeEvent, uint64_t, bool);

  // XXX Causes "handler error" on the parent side.
  //case 44: return FUZZY_CALL3(mPBrowserChild, AsyncAuthPrompt, nsCString, nsString, uint64_t);

  //case 45: return FUZZY_CALL2(mPBrowserChild, LookUpDictionary, nsString, FontRange[], bool, LayoutDeviceIntPoint);
  //case 46: return FUZZY_CALL2(mPBrowserChild, __delete__);
  //case 47: return FUZZY_CALL2(mPBrowserChild, ReplyKeyEvent, WidgetKeyboardEvent);
  //case 48: return FUZZY_CALL2(mPBrowserChild, RequestNativeKeyBindings, WidgetKeyboardEvent, MaybeNativeKeyBinding&);
  case 49: return FUZZY_CALL6(mPBrowserChild, SynthesizeNativeKeyEvent, int32_t, int32_t, uint32_t, nsString, nsString, uint64_t);
  //case 50: return FUZZY_CALL2(mPBrowserChild, SynthesizeNativeMouseEvent, LayoutDeviceIntPoint, uint32_t, uint32_t, uint64_t);
  //case 51: return FUZZY_CALL2(mPBrowserChild, SynthesizeNativeMouseMove, LayoutDeviceIntPoint, uint64_t);
  //case 52: return FUZZY_CALL2(mPBrowserChild, SynthesizeNativeMouseScrollEvent, LayoutDeviceIntPoint, uint32_t, double, double, double, uint32_t, uint32_t, uint64_t);
  //case 53: return FUZZY_CALL2(mPBrowserChild, SynthesizeNativeTouchPoint, uint32_t, TouchPointerState, LayoutDeviceIntPoint, double, uint32_t, uint64_t);
  //case 54: return FUZZY_CALL2(mPBrowserChild, SynthesizeNativeTouchTap, LayoutDeviceIntPoint, bool, uint64_t);
  case 55: return FUZZY_CALL1(mPBrowserChild, ClearNativeTouchSequence, uint64_t);
  case 56: return FUZZY_CALL1(mPBrowserChild, GetTabCount, uint32_t&);
  //case 57: return FUZZY_CALL1(mPBrowserChild, AccessKeyNotHandled, WidgetKeyboardEvent);
  case 58: return FUZZY_CALL0(mPBrowserChild, RemotePaintIsReady);
  case 59: return FUZZY_CALL1(mPBrowserChild, ForcePaintNoOp, uint64_t);

  // XXX Causes handler error on the parent side.
  //case 60: return FUZZY_CALL5(mPBrowserChild, SetDimensions, uint32_t, int32_t, int32_t, int32_t, int32_t);

  //case 61: return FUZZY_CALL2(mPBrowserChild, DispatchWheelEvent, WidgetWheelEvent);
  //case 62: return FUZZY_CALL2(mPBrowserChild, DispatchMouseEvent, WidgetMouseEvent);
  //case 63: return FUZZY_CALL2(mPBrowserChild, DispatchKeyboardEvent, WidgetKeyboardEvent);
  //case 64: return FUZZY_CALL2(mPBrowserChild, InvokeDragSession, IPCDataTransfer[], uint32_t, uint8_t, LayoutDeviceIntRect);

  // XXX Causes handler error on the parent side.
  //case 65: return FUZZY_CALL2(mPBrowserChild, AudioChannelActivityNotification, uint32_t, bool);

  case 66: return FUZZY_CALL0(mPBrowserChild, EnsureLayersConnected);
  case 67: return FUZZY_CALL3(mPBrowserChild, SHistoryUpdate, uint32_t, uint32_t, bool);
  case 68: return FUZZY_CALL1(mPBrowserChild, RequestCrossBrowserNavigation, uint32_t);
  case 69: return FUZZY_CALL1(mPBrowserChild, IsParentWindowMainWidgetVisible, bool&);
  }

  LOG(("Unexecuted callIndex: %d", callIndex));
  return true;
}

}
