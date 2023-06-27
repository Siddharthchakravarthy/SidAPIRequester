#define CURL_STATICLIB

#include "GUI.h"
#include <thread>

#include <iostream>
#include <string>
#include "curl/curl.h"


size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* response)
{
    size_t totalSize = size * nmemb;
    response->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

int wWinMain(HINSTANCE Instance, HINSTANCE PreviousInstance, PWSTR Arguments, int CommandShow) {
    /*CURL* curl;
    CURLcode res;

    std::string response;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://jsonplaceholder.typicode.com/users");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
        else {
            std::cout << "Response:\n" << response << std::endl;
        }

        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();*/

	GUI::CreateTheWindow("APIRequest Window", "APIRequest Class");
	GUI::CreateDevice();
	GUI::CreateImGui();
	
	while (GUI::exit) {
		GUI::BeginRender();
		GUI::Render();

        
        if (ImGui::Begin("Test API", &GUI::exit, ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove)) {
            ImGui::SetCursorPos({ 200, 50 });
            ImGui::Text("This is a Basic Get Request from C++, \n idna bari test maadakke maadideeni");
            ImGui::SetCursorPos({ 200, 80 });
            ImGui::BulletText("Idna Bere Credentials itkondu use maadkoboodu");
            ImGui::SetCursorPos({ 200, 95 });
            ImGui::BulletText("Idu Thumba Easy to use, can be customized in the future");
            ImGui::SetCursorPos({ 300, 150 });
            if (ImGui::Button("GetRequest", {100, 20})) {

            }
        }
		
		GUI::EndRender();

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	GUI::DestroyImGui();
	GUI::DestroyDevice();
	GUI::DestroyTheWindow();
	
	return EXIT_SUCCESS;
}



// Callback function to handle the response data
