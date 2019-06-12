#include <RestServer.h>

RestServer::RestServer(uint ledPin) : m_server(m_port)
{
    m_ledPin = ledPin;
}

void RestServer::startWifi()
{
    WiFiManager wifiManager;
    wifiManager.setTimeout(100);
    wifiManager.autoConnect("Neo IoT");
    m_server.begin();
}

void RestServer::wifiConnected(WiFiManager *cbWM)
{
    Serial.println("Connected to wifi " + cbWM->getConfigPortalSSID());
}

RestCommand RestServer::handle()
{
    WiFiClient client = m_server.available();
    RestCommand emptyCommand;
    emptyCommand.empty = true;

    if (client)
    {
        String cmd = readLine(client);
        Serial.println(cmd);
        handleCommand(cmd);
        sendOK(client);
        client.flush();
        client.stop();
        return parse(cmd);
    }
    return emptyCommand;
}

RestCommand RestServer::parse(String command)
{
    RestCommand restCommand;
    restCommand.empty = false;
    int start = command.indexOf("GET /") + 5;
    int stop = command.indexOf(" HTTP/");
    String chunk = command.substring(start, stop);

    restCommand.path = chunk.substring(0, chunk.lastIndexOf("/"));
    restCommand.value = chunk.substring(chunk.lastIndexOf("/") + 1);

    return restCommand;
}

String RestServer::readLine(WiFiClient client)
{
    String line = "";
    client.setTimeout(1);
    while (client.connected())
    {
        while (client.available() > 0)
        {
            char c = client.read();
            if (c == '\n')
                return line;
            line += c;
        }
    }
    return "";
}

void RestServer::handleCommand(String cmd)
{
    if (cmd.indexOf("GET /LED/ON") >= 0)
    {
        digitalWrite(m_ledPin, LOW);
    }
    else if (cmd.indexOf("GET /LED/OFF") >= 0)
    {
        digitalWrite(m_ledPin, HIGH);
    }
}

void RestServer::sendOK(WiFiClient client)
{
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println("Connection: close");
    client.println();
}