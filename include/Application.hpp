#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Window.hpp"

class Application
{
    public:
            Application(const std::string& programDir);
               
        void        run();
    
    private:
        Window          mWindow;
        std::string     mProgramDir;
};

#endif