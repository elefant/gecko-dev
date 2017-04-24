#ifndef mozilla_FuzzTraitsPBrowser_h
#define mozilla_FuzzTraitsPBrowser_h

#include "mozilla/fuzzing/FuzzTraitsPrimitives.h"
#include "ipc/nsGUIEventIPC.h"
#include "nsBaseWidget.h"

using namespace mozilla;
using namespace widget;

template<>
struct FuzzTraits<ContentCache>
{
  using ParamType = ContentCache;
  // XXX Or we just return ContentCache and leave it sliced?
  inline static ContentCacheInChild Fuzz();
};

template<>
struct FuzzTraits<WritingMode>
{
  using ParamType = WritingMode;
  inline static ParamType Fuzz();
};

template<>
struct FuzzTraits<nsIWidget>
{
  using ParamType = nsIWidget;
  inline static ParamType* Fuzz();
};

template<>
struct FuzzTraits<LayoutDeviceIntPoint>
{
  using ParamType = LayoutDeviceIntPoint;
  inline static ParamType Fuzz();
};

template<>
struct FuzzTraits<InputContext>
{
  using ParamType = InputContext;
  inline static ParamType Fuzz();
};

template<>
struct FuzzTraits<IMEState>
{
  using ParamType = IMEState;
  inline static ParamType Fuzz();
};

template<>
struct FuzzTraits<IMENotification>
{
  using ParamType = IMENotification;
  inline static ParamType Fuzz();
};

template<>
struct FuzzTraits<IMEMessage>
{
  using ParamType = IMEMessage;
  inline static ParamType Fuzz();
};

////////////////////////////////////////////////////////////////
// Implementations

auto
FuzzTraits<ContentCache>::Fuzz() -> ContentCacheInChild
{
  ContentCacheInChild cache;

  cache.SetSelection(FuzzTraits<nsIWidget>::Fuzz(), // nsIWidget aWidget
                     FuzzTraits<uint32_t>::Fuzz(), // aStartOffset
                     FuzzTraits<uint32_t>::Fuzz(), // aLength
                     FuzzTraits<bool>::Fuzz(), // aReversed
                     FuzzTraits<WritingMode>::Fuzz()); // aWritingMode

  return cache;
}

auto
FuzzTraits<WritingMode>::Fuzz() -> ParamType
{
  return WritingMode();
}

namespace {

class FuzzedWidget : public nsBaseWidget
{
public:
  virtual MOZ_MUST_USE nsresult
  Create(nsIWidget* aParent,
         nsNativeWidget aNativeParent,
         const LayoutDeviceIntRect& aRect,
         nsWidgetInitData* aInitData) override
  {
    return FuzzTraits<nsresult>::Fuzz();
  }

  virtual void Show(bool aState) override {}
  virtual bool IsVisible() const override { return FuzzTraits<bool>::Fuzz(); }
  virtual void Move(double aX, double aY) override {}
  virtual void Resize(double, double, bool) override {}
  virtual void Resize(double, double, double, double, bool) override {}
  virtual void Enable(bool aState) override {}
  virtual bool IsEnabled() const override { return FuzzTraits<bool>::Fuzz(); }
  virtual nsresult SetFocus(bool aRaise) override { return NS_OK; }
  virtual nsresult ConfigureChildren(const nsTArray<Configuration>&) override { return NS_OK; }
  virtual void Invalidate(const LayoutDeviceIntRect& aRect) override {}
  virtual void* GetNativeData(uint32_t aDataType) override { return nullptr; }
  virtual nsresult SetTitle(const nsAString& aTitle) override { return FuzzTraits<nsresult>::Fuzz(); }
  virtual LayoutDeviceIntPoint WidgetToScreenOffset() override
  {
    return FuzzTraits<LayoutDeviceIntPoint>::Fuzz();
  }
  virtual nsresult DispatchEvent(mozilla::WidgetGUIEvent* event,
                                 nsEventStatus & aStatus) override
  {
    return FuzzTraits<nsresult>::Fuzz();
  }
  virtual void SetInputContext(const InputContext& aContext,
                               const InputContextAction& aAction) override {}
  virtual InputContext GetInputContext() override
  {
    return FuzzTraits<InputContext>::Fuzz();
  };
};

} // end of unnamed namespace.

auto
FuzzTraits<nsIWidget>::Fuzz() -> ParamType*
{
  return new FuzzedWidget();
}

auto
FuzzTraits<LayoutDeviceIntPoint>::Fuzz() -> ParamType
{
  return LayoutDeviceIntPoint(FuzzTraits<int32_t>::Fuzz(),
                              FuzzTraits<int32_t>::Fuzz());
}

auto
FuzzTraits<InputContext>::Fuzz() -> ParamType
{
  InputContext inputContext;
  AutoTypeFuzz(inputContext.mIMEState);
  AutoTypeFuzz(inputContext.mHTMLInputType);
  AutoTypeFuzz(inputContext.mHTMLInputInputmode);
  AutoTypeFuzz(inputContext.mActionHint);
  return inputContext;
}

auto
FuzzTraits<IMEState>::Fuzz() -> ParamType
{
  IMEState state;
  state.mEnabled = static_cast<IMEState::Enabled>(FuzzTraits<uint8_t>::Fuzz());
  state.mOpen = static_cast<IMEState::Open>(FuzzTraits<uint8_t>::Fuzz());
  return state;
}

auto
FuzzTraits<IMENotification>::Fuzz() -> ParamType
{
  IMENotification noti(FuzzTraits<IMEMessage>::Fuzz());
  return noti;
}

auto
FuzzTraits<IMEMessage>::Fuzz() -> ParamType
{
  return static_cast<IMEMessage>(FuzzTraits<IMEMessageType>::Fuzz());
}

#endif














