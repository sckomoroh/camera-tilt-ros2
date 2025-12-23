#include "TiltDriver.h"

#include <nlohmann/json.hpp>

#define SEND_ANGLE_TEMPLATE "{\"C\": 301, \"D\": {\"pan\": %f, \"tilt\": %f}}"
#define GET_STATE_TEMPLATE "{\"C\": 302}"
#define DRIVER_URI "http://192.168.0.147:8010/"

using json = nlohmann::json;

static size_t WriteCb(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    auto *out = static_cast<std::string *>(userdata);
    out->append(ptr, size * nmemb);
    return size * nmemb;
}

void TiltDriver::sendAngles(double tiltAngle, double cameraAngle)
{
    char buffer[1024] = {0};
    sprintf(buffer, SEND_ANGLE_TEMPLATE, cameraAngle * 180.0 / 3.14, tiltAngle * 180.0 / 3.14);
    httpPostJson(DRIVER_URI, buffer, 500);
}

void TiltDriver::getStates(double &tiltAngle, double &cameraAngle, double &tiltVelocity, double &cameraVelocity)
{
    auto response = httpPostJson(DRIVER_URI, "{\"C\": 303}", 500);
    if (response.empty() == false)
    {
        json j = json::parse(response);
        auto responseData = j["data"];
        auto tilt = responseData["tilt"];
        auto pan = responseData["pan"];
        tiltAngle = tilt["pos"].get<double>() * 3.14 / 180.0;
        tiltVelocity = tilt["speed"];
        cameraAngle = pan["pos"].get<double>() * 3.14 / 180.0;
        cameraVelocity = pan["speed"];
    }
}

std::string TiltDriver::httpPostJson(const std::string &url, const std::string &json_body, long timeout_ms)
{
    CURL *curl = curl_easy_init();
    if (!curl)
        throw std::runtime_error("curl_easy_init failed");

    std::string response;

    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "Accept: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_body.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)json_body.size());

    // таймауты как в requests timeout=0.5
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, timeout_ms);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, timeout_ms);

    // получить тело ответа
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode rc = curl_easy_perform(curl);

    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (rc != CURLE_OK)
    {
        // throw std::runtime_error(std::string("curl error: ") + curl_easy_strerror(rc));
        fprintf(stderr, "[ERROR] <DRIVER> curl error: %s\n", curl_easy_strerror(rc));
    }
    if (http_code < 200 || http_code >= 300)
    {
        // throw std::runtime_error("HTTP error code: " + std::to_string(http_code) + ", body=" + response);
        fprintf(stderr, "[ERROR] <DRIVER> HTTP error code: %ld, body=%s\n", http_code, response.c_str());
    }

    return response;
}
