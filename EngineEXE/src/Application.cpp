// StudioSim.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"

#include <BasicIncludes.h>
#include "Quack.h"

int main(void)
{
    QuackEngine::Log::Init();
    Quack::InitEngine();

    while (Quack::s_running)
    {
        Quack::Update();
        Quack::RenderUpdate();
        Quack::GetAudioEngine()->Update();
    }
    Quack::ShutDown();
    return 0;
}