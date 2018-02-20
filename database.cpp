#include <list>
#include <iostream>
#include <sstream>
#include <string.h>
#include <stdlib.h>

#include "database.h"
#include "vhc.h"
#include "utils.h"

Database::Database(string hostAddress, int port, string dbName, string user, string password) {
    this->hostAddress = hostAddress;
    this->port = port;
    this->dbName = dbName;
    this->user = user;
    this->password = password;
    
    nQueries = 0;
}

Database::~Database() {
    
}

PGconn* Database::connect() {
    PGconn* conn;
    stringstream conninfo;
    conninfo << "hostaddr=" << hostAddress << " port=" << port << " dbname=" << dbName << " user=" << user << " password=" << password;
    
    //Make a connection to the database
    conn = PQconnectdb(conninfo.str().c_str());
    
    //Check to see that the backend connection was successfully made
    if(PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
        
        PQfinish(conn);
        return 0;
    }
    
//    cout << "Conectado a " << conninfo.str() << "\n";
    
    return conn;
}

void Database::loadHashes(string hashingRefs, string begin, string end, int hashWindow) {
    PGconn* conn = this->connect();
    PGresult* res;
    int i;
    
    if(!conn) {
        return;
    }
    
    string query = getHashingQuery(hashingRefs, begin, end);
    cout << "QUERY >> " << query << "\n";
    res = PQexec(conn, query.c_str());
    
    if(PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "'%s' failed: %s", query.c_str(), PQerrorMessage(conn));
        PQclear(res);
        PQfinish(conn);
        return;
    }
    
    int size = PQntuples(res);
    
    // next, print out the rows
    for(i = 0; i < size; i++) {
        char* hashingIdString = PQgetvalue(res, i, 0);
        char* hashingExtRef = PQgetvalue(res, i, 1);
        char* timeRefString = PQgetvalue(res, i, 2);
        
        uint64_t hashingId = atol(hashingIdString);
        uint64_t timeRef = atol(timeRefString);
        string extRef = hashingExtRef;
        
        printf("%ld %s\n", timeRef, extRef.c_str());
        
        //--------------------------
        
        string query2 = getVHCQuery(hashingId);
        PGresult* res2 = PQexec(conn, query2.c_str());
        int size2 = PQntuples(res2);
        cout << "QUERY2 >> " << query2 << "\n";
        printf("size: %ld\n", size2);
        
        list<VHC*> vhcList;
        int j;
        for(j = 0; j < size2; j++) {
            char* hashTimeString = PQgetvalue(res2, j, 0);
            char* vhcString = PQgetvalue(res2, j, 1);
            
            uint32_t relativeTime = atoi(hashTimeString);
            uint64_t vhc = atol(vhcString);
            
            vhcList.push_back(new VHC(relativeTime, vhc, 0));
            
//            printf("%d %ld\n", relativeTime, vhc);
        }
        stringstream ss;
        ss << "vhcs/" << hashingId << ".vhc";

        saveVHCs(ss.str(),
                extRef,
                "30 0,0,0,0 32x32 4x4x4 1 1",
                timeRef,
                &vhcList);
        
        //exit(0);
    }
    
    PQclear(res);
    
    // Close the connection to the database and cleanup
    PQfinish(conn);
}

string Database::getHashingQuery(string hashingRefs, string beginString, string endString) {
    stringstream query;
    query << "SELECT id,ext_ref,time_ref FROM vsr_hashing WHERE task_status=2";
//query << " AND id=211125";
    //Filtro por nome de canal ou id de obra
    if(!hashingRefs.empty()) {
        query << " AND ext_ref IN (" << hashingRefs << ")";
    }

    //Filtro temporal (início)
    if(!beginString.empty()) {
        query << " AND time_ref>=" << beginString;
    }

    //Filtro temporal (fim)
    if(!endString.empty()) {
        query << " AND time_ref<=" << endString;
    }
    
    query << " ORDER BY ext_ref,time_ref";
    
    return query.str();
}

string Database::getVHCQuery(uint64_t hashing_id) {
    stringstream query;
    query << "SELECT hash_time,vhc FROM vsr_vhc WHERE hashing_id=" << hashing_id << " ORDER BY group_prefix,hash_time";
    return query.str();
}