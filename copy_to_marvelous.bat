@echo off
set /p Input=Enter your Marvelous repo path: 
echo %Input%
@echo on

xcopy /y ".\RenderingScenarioTest\DeviceMemory.cpp" "%Input%\Source\Renderer\RenderSystem\DeviceMemory.cpp"
xcopy /y ".\RenderingScenarioTest\DeviceMemory.h" "%Input%\Source\Renderer\RenderSystem\DeviceMemory.h"
xcopy /y ".\RenderingScenarioTest\Framebuffer.cpp" "%Input%\Source\Renderer\RenderSystem\Framebuffer.cpp"
xcopy /y ".\RenderingScenarioTest\Framebuffer.h" "%Input%\Source\Renderer\RenderSystem\Framebuffer.h"