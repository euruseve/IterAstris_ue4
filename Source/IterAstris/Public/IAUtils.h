#pragma once

class IAUtils
{
public:
    template <typename T> 
    static T* GetIAPlayerComponent(class APawn* PlayerPawn)
    {
        if (!PlayerPawn)
            return nullptr;

        const auto Component = PlayerPawn->GetComponentByClass(T::StaticClass());
        return Cast<T>(Component);
    }
};