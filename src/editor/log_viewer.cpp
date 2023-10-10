#include "log_viewer.h"
#include "imgui.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

char searchBuffer[128] = { 0 };
std::string combinedLog;
std::vector<LogEntry> logs;
std::vector<LogEntry> filteredLogs;
bool autoScrollToLast = true;

void RebuildLog() {
    std::ostringstream combined;
    for (const auto& logEntry : filteredLogs) {
        combined << logEntry.message << "\n";
    }
    combinedLog = combined.str();
}


void FilterLogs() {
    if (strlen(searchBuffer) == 0) {
        // If search buffer is empty, show all logs
        filteredLogs = logs;
    } else {
        filteredLogs.clear();
        std::string searchStr(searchBuffer);
        for (const auto& logEntry : logs) {
            if (logEntry.message.find(searchStr) != std::string::npos) {
                filteredLogs.push_back(logEntry);
            }
        }
    }
    RebuildLog();
}

LogView::LogView(std::shared_ptr<LogSink> sink) {

    if (sink == NULL) {
        sink = std::make_shared<EditorSink>();
    }

    auto editorSinkPtr = std::static_pointer_cast<EditorSink>(sink);

    editorSinkPtr->AttachSinkCallback([](const LogEntry& entry) {
        logs.push_back(entry);
        FilterLogs();
    });
}

void LogView::RenderWindow() {
    static bool s = true;
    if (ImGui::Begin("Log Viewer", &s)) {
        ImGui::Text("Search:");                                                // Display the label
        ImGui::SameLine();                                                     // Ensure the next widget is on the same line
        ImGui::InputText("##SearchField", searchBuffer, sizeof(searchBuffer)); // Using an ID to differentiate this InputText from others

        ImGui::Separator();

        FilterLogs();

        ImGui::BeginChild("LogRegion", ImVec2(0, ImGui::GetContentRegionAvail().y), false, ImGuiWindowFlags_HorizontalScrollbar);

        for (const auto& logEntry : filteredLogs) {
            switch (logEntry.type) {
            case CLOG_INFO:
                ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "%s", logEntry.message.c_str());
                break;
            case CLOG_WARNING:
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", logEntry.message.c_str());
                break;
            case CLOG_ERROR:
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%s", logEntry.message.c_str());
                break;
            default:
                ImGui::Text("%s", logEntry.message.c_str());
                break;
            }
        }

        if (autoScrollToLast) {
            ImGui::SetScrollHereY(1.0f);
        }


        ImGui::EndChild();
    }
    ImGui::End();
}
