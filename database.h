#ifndef DATABASE_H
#define	DATABASE_H

#include <postgresql/libpq-fe.h>
#include <string>
#include <list>
#include <stdint.h>

using namespace std;

/**
 *
 * @author Eduardo
 */
class Database {
    private:
        string hostAddress;
        int port;
        string dbName;
        string user;
        string password;
        string getVHCQuery(uint64_t hashingId);
        string getVHCQueryWithBounds(string hashingRefs, string videoTimeBeginString, string videoTimeEndString, int hashWindow, uint8_t prefix);
        
        int nQueries;
    
    public:
        double fetchTime;
        double loadTime;
        
        Database(string hostAddress, int port, string dbName, string user, string password);
        ~Database();
        PGconn* connect();
        void loadHashes(string hashingRefs, string videoTimeBeginString, string videoTimeEndString, int hashWindow);
        string getHashingQuery(string hashingRefs, string beginString, string endString);
};

#endif