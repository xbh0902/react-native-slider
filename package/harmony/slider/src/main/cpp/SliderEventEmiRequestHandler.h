/**
 * MIT License
 *
 * Copyright (C) 2023 Huawei Device Co., Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include "RNOH/ArkJS.h"
#include "RNOH/EventEmitRequestHandler.h"
#include "EventEmitters.h"


using namespace facebook;
namespace rnoh {

enum SliderEventType {
    VALUE_CHANGE = 0,
    SLIDING_START = 1,
    SLIDING_COMPLETE = 2
};

SliderEventType getSliderEventType(ArkJS &arkJs, napi_value eventObject) {
    auto eventType = arkJs.getString(arkJs.getObjectProperty(eventObject, "type"));
    if (eventType == "SliderValueChange") {
        return SliderEventType::VALUE_CHANGE;
    } else if (eventType == "SliderSlidingStart") {
        return SliderEventType::SLIDING_START;
    } else if (eventType == "SliderSlidingComplete") {
        return SliderEventType::SLIDING_COMPLETE;
    } else {
        throw std::runtime_error("Unknown Slider event type");
    };
};

class SliderEventEmitRequestHandler : public EventEmitRequestHandler {
    public:
        void handleEvent(EventEmitRequestHandler::Context const &ctx) override {
            if (ctx.eventName != "RNCSlider") {
                return;
            }
            ArkJS arkJs(ctx.env);
            auto eventEmitter = ctx.shadowViewRegistry->getEventEmitter<react::RNCSliderEventEmitter>(ctx.tag);
            if (eventEmitter == nullptr) {
                return;
            }
            auto eventType = getSliderEventType(arkJs, ctx.payload);
            switch (eventType)
            {
            case SliderEventType::VALUE_CHANGE: {
                facebook::react::Float value = arkJs.getDouble(arkJs.getObjectProperty(ctx.payload, "value"));
                react::RNCSliderEventEmitter::OnChange event1 = {value};
                eventEmitter->onChange(event1);
                react::RNCSliderEventEmitter::OnRNCSliderValueChange event2 = {value};
                eventEmitter->onRNCSliderValueChange(event2);
                break;
            }
            case SliderEventType::SLIDING_START: {
                facebook::react::Float value = arkJs.getDouble(arkJs.getObjectProperty(ctx.payload, "value"));
                react::RNCSliderEventEmitter::OnRNCSliderSlidingStart event = {value};
                eventEmitter->onRNCSliderSlidingStart(event);
                break;
            }
            case SliderEventType::SLIDING_COMPLETE: {
                facebook::react::Float value = arkJs.getDouble(arkJs.getObjectProperty(ctx.payload, "value"));
                react::RNCSliderEventEmitter::OnRNCSliderSlidingComplete event = {value};
                eventEmitter->onRNCSliderSlidingComplete(event);
                break;
            }
            default:
                break;
            }
        };
};
} // namespace rnoh