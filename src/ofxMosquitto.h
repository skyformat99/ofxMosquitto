#include "ofMain.h"
#include "mosquittopp.h"

struct ofxMosquittoMessage
{
    int mid;
    string topic;
	void *payload;
	int payloadlen;
	int qos;
	bool retain;
    
    const string payloadAsString()
    {
        return static_cast<string>((const char *)payload);
    }
};

class ofxMosquitto : public mosqpp::mosquittopp, public ofThread
{

public:
    
    ofxMosquitto();
    ofxMosquitto(const ofxMosquitto& mom);
    ofxMosquitto & operator=(const ofxMosquitto& mom);
    ofxMosquitto(string clientID, string host, int port, bool cleanSession=true);
    ~ofxMosquitto();
    
    void setup(string host, int port, int keepAlive = 60);
    int reinitialise(string clientID, bool cleanSession);
    int connect();
    int connect(string bindAddress);
    int reconnect();
    int disconnect();
    int publish(string topic, string payload);
    int subscribe(int mid, string sub, int qos = 0);
    int unsubscribe(int mid, string sub);
    
    void threadedFunction();
    void start();
    void stop();
    
    string getClientID() { return clientID; }
    string getHost()     { return host; }
    int    getPort()     { return port; }
    string getUsername() { return username; }
    string getPassword() { return password; }
    
    bool isConnected() { return bConnected; };
    
    void setUsernameAndPassword(string username, string password);
    void setKeepAlive(int keepAlive);
    void setAutoReconnect(bool reconnect);
    void setUserdata(void *userdata);
    
    ofEvent<int> onConnect;
    ofEvent<ofxMosquittoMessage> onMessage;
    ofEvent<int> onDisconnect;
    ofEvent<int> onPublish;
    ofEvent<int> onSubscribe;
    ofEvent<int> onUnsubscribe;
    
private:

    string clientID;
    string host;
    int port;
    string username;
    string password;
    int keepAlive;
    bool bConnected;
    bool bAutoReconnect;
    void *userdata;
    string timestamp;
    
    void on_connect(int rc);
    void on_disconnect(int rc);
    void on_message(const struct mosquitto_message *message);
    void on_publish(int rc);
    void on_subscribe(int mid, int qos_count, const int *granted_qos);
    void on_unsubscribe(int mid);
    void on_log(int level, const char *str);
    void on_error();
};