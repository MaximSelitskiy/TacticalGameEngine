#pragma once

#include "../../core/interfaces/ILogger.h"
#include "logger_file.h"

#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <memory>

namespace Engine::Infrastructure::Logging
{
    class LoggerFTXUI : public Core::Interfaces::ILogger
    {
    private:
        std::unique_ptr<LoggerFile> file_logger_;

        void showAlert(const std::string &title, const std::string &message, ftxui::Color title_color)
        {
            using namespace ftxui;

            auto screen = ScreenInteractive::Fullscreen();
            
            auto ok_button = Button(" OK ('q') ", [&]() { screen.Exit(); });

            auto alert_renderer = Renderer(ok_button, [&]() -> Element {
                return window(
                    text(" " + title + " ") | bold | color(title_color) | hcenter,
                    vbox({
                        text(message) | hcenter,
                        separator(),
                        ok_button->Render() | hcenter
                    })
                ) | center;
            });

            auto event_handler = CatchEvent(alert_renderer, [&](Event event) {
                if (event == Event::Return || 
                    event == Event::Character("q") || 
                    event == Event::Character("Q") || 
                    event == Event::Escape) 
                {
                    screen.Exit();
                    return true;
                }
                return false;
            });

            screen.Loop(event_handler);
        }

    public:
        explicit LoggerFTXUI(const std::string &log_filename = "logs/app.log")
            : file_logger_(std::make_unique<LoggerFile>(log_filename)) {}

        ~LoggerFTXUI() override = default;

        LoggerFTXUI(const LoggerFTXUI &) = delete;
        LoggerFTXUI &operator=(const LoggerFTXUI &) = delete;
        LoggerFTXUI(LoggerFTXUI &&other) noexcept = default;
        LoggerFTXUI &operator=(LoggerFTXUI &&other) noexcept = default;

        void info(const std::string &message) override
        {
            file_logger_->info(message);
        }

        void warn(const std::string &message) override
        {
            file_logger_->warn(message);
            showAlert("WARNING", message, ftxui::Color::Yellow);
        }

        void errror(const std::string &message) override
        {
            file_logger_->errror(message);
            showAlert("ERROR", message, ftxui::Color::Red);
        }
    };
}