#ifndef NetworkManager_H
#define NetworkManager_H

class NetworkManager {
public:
    virtual ~NetworkManager( void );

	void initialise( NETWORK_TYPE eType );

    void connect( void );
	void disconnect( void );
    void updateNetworkSystem( unsigned long lTimeElapsed );

    void sendText( void );
	void sendObject( void );

	void receiveText( void );
    void reveiveObject( void );

	bool isConnected( void );
    NETWORK_TYPE getLocalType( void );

    static NetworkManager* getSingletonPtr( void );

    typedef enum {
        NETWORK_TYPE_HOST_LAN,
        NETWORK_TYPE_CLIENT_LAN,
        NETWORK_TYPE_DEDICATED_LAN,
        NETWORK_TYPE_HOST_INET,
        NETWORK_TYPE_CLIENT_INET,
        NETWORK_TYPE_DEDICATED_INET,
        NETWORK_TYPE_UNDEFINED
    } NETWORK_TYPE;
private:
    NetworkManager( void );
    NetworkManager( const NetworkManager& ) { }
    NetworkManager & operator = ( const NetworkManager& );

	void sendPacket( void );
	void receivePacket( void );

    NETWORK_TYPE eLocalType;

    static NetworkManaget *mNetworkManager;
};
#endif