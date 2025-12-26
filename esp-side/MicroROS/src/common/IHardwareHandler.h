#pragma once

template <class TData>
class IHardwareHandler {
public:
    virtual ~IHardwareHandler() = default;

public:
    virtual void onCommandReceive(const TData& data) = 0;
    virtual void init() = 0;
};

template <class TData>
class IHardwareFeedbackProvider {
public:
    virtual ~IHardwareFeedbackProvider() = default;

public:
    virtual TData getFeedback() = 0;
};
