#include "PanelConsole.h"
#include "App.h"
#include "Gui.h"
#include "imgui.h"

#include "Log.h"

PanelConsole::PanelConsole(PanelType type, std::string name) : Panel(type, name) {}

PanelConsole::~PanelConsole() {}

bool PanelConsole::Draw()
{
	ImGuiWindowFlags consoleFlags = 0;
	consoleFlags |= ImGuiWindowFlags_NoFocusOnAppearing;

	if (ImGui::Begin("Console", &enabled, consoleFlags))
	{
		if (ImGui::SmallButton("Clear"))
			app->CleanLogs();

		ImGui::Separator();

		ImGuiWindowFlags scrollFlags = 0;
		scrollFlags |= ImGuiWindowFlags_HorizontalScrollbar;
		scrollFlags |= ImGuiWindowFlags_AlwaysVerticalScrollbar;

		if (ImGui::BeginChild("Scrollbar", ImVec2(0, 0), false, scrollFlags))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 15));

			std::string logType;

			for (const auto& log : app->GetLogs())
			{
				switch (log.type)
				{
				case LogType::LOG_INFO:
					logType = "";
					break;

				case LogType::LOG_ASSIMP:
					logType = "[ASSIMP] ";
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(190, 64, 190, 255));
					break;

				case LogType::LOG_OK:
					logType = "[OK] ";
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 255, 0, 255));
					break;

				case LogType::LOG_WARNING:
					logType = "[WARNING] ";
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(255, 255, 0, 255));
					break;

				case LogType::LOG_ERROR:
					logType = "[ERROR] ";
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(255, 0, 0, 255));
					break;
				}
				
				if (log.message[0] == '-')
					logType.insert(0, "\t");

				ImGui::Text(logType.c_str());
				ImGui::PopStyleColor();
				ImGui::SameLine();
				ImGui::Text(log.message.c_str());
			}

			ImGui::PopStyleVar();

			if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
				ImGui::SetScrollHereY(1.0f);

			ImGui::EndChild();
		}

		ImGui::End();
	}

	return true;
}