/* Database implementations file */
#include "Database.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int ishex(int x) {
   return (x >= '0' && x <= '9') ||
          (x >= 'a' && x <= 'f') ||
          (x >= 'A' && x <= 'F');
}

int decode(const char *s, char *dec) {
        char *o;
        const char *end = s + strlen(s);
        unsigned int c;

        for (o = dec; s <= end; o++) {
                c = *s++;
                if (c == '+') c = ' ';  
                else if (c == '%' && (  !ishex(*s++)    ||
                                        !ishex(*s++)    ||
                                        !sscanf(s - 2, "%2x", &c)))
                        return -1;

                if (dec) *o = c;
        }
        return o - dec;
}

_id_t storeElection(sqlite3 *db, Date deadline) {
   _id_t id = 0;
   sqlite3_stmt *stmt;
   const char *sql = "INSERT INTO Election(deadline_day,deadline_mon,\
                      deadline_year,status) VALUES (?, ?, ?, ?)";
   sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
   sqlite3_bind_int(stmt, 1, deadline.day);
   sqlite3_bind_int(stmt, 2, deadline.month);
   sqlite3_bind_int(stmt, 3, deadline.year);
   sqlite3_bind_int(stmt, 4, INACTIVE);
   sqlite3_step(stmt);
   if (sqlite3_finalize(stmt) == SQLITE_OK) {
      id = (_id_t)sqlite3_last_insert_rowid(db);
   }
   return id;
}

_id_t storeOffice(sqlite3 *db, _id_t election, char *name) {
   _id_t id = 0;
   sqlite3_stmt *stmt;
   const char *sql = "INSERT INTO Office(name, election) VALUES (?, ?)";
   sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
   sqlite3_bind_text(stmt, 1, name, (int)strnlen(name, MAX_NAME_LEN),
                     SQLITE_STATIC);
   sqlite3_bind_int(stmt, 2, election);
   sqlite3_step(stmt);
   if (sqlite3_finalize(stmt) == SQLITE_OK) {
      id = (_id_t)sqlite3_last_insert_rowid(db);
   }
   return id;
}

_id_t storeCandidate(sqlite3 *db, _id_t office, char *name) {
   _id_t id = 0;
   sqlite3_stmt *stmt;
   const char *sql = "INSERT INTO Candidate(name,votes,office)\
                      VALUES (?, ?, ?)";
   sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
   sqlite3_bind_text(stmt, 1, name, (int)strnlen(name, MAX_NAME_LEN),
                     SQLITE_STATIC);
   sqlite3_bind_int(stmt, 2, 0);
   sqlite3_bind_int(stmt, 3, office);
   sqlite3_step(stmt);
   if (sqlite3_finalize(stmt) == SQLITE_OK) {
      id = (_id_t)sqlite3_last_insert_rowid(db);
   }
   return id;
}

void addZip(sqlite3 *db, _id_t office, int zip) {
   sqlite3_stmt *stmt;
   const char *sql = "INSERT INTO AllowedZip(zip,office) VALUES (?, ?)";
   sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
   sqlite3_bind_int(stmt, 1, zip);
   sqlite3_bind_int(stmt, 2, office);
   sqlite3_step(stmt);
   sqlite3_finalize(stmt);
}

bool checkZip(sqlite3 *db, _id_t office, int zip) {
   int count;
   sqlite3_stmt *stmt;
   const char *sql = "SELECT COUNT(*) FROM AllowedZip WHERE\
                      zip=? AND office=?";
   sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
   sqlite3_bind_int(stmt, 1, zip);
   sqlite3_bind_int(stmt, 2, office);
   sqlite3_step(stmt);
   count = sqlite3_column_int(stmt, 0);
   sqlite3_finalize(stmt);
   return count > 0;
}

_id_t storeVoter(sqlite3 *db, char*name, char*county, int zip, Date dob) {
   _id_t id = 0;

   char nameout[strlen(name)+1];  
   decode(name, nameout);

   char buffer[1000];   
   char comma=',';
   char lr='"';
   snprintf(buffer, sizeof buffer, "INSERT INTO Registration(name, county,zip,\
                      dob_day,dob_mon,dob_year) VALUES (");

   strcat(buffer, &lr);
   strcat(buffer, nameout);
   strcat(buffer, &lr);
   strncat(buffer, &comma, 1);
   strcat(buffer, &lr);
   strcat(buffer, county);
   strcat(buffer, &lr);
   strncat(buffer, &comma, 1);
   char str[6];
   sprintf(str, "%d", zip);
   strcat(buffer, str);
   strncat(buffer, &comma, 1);

   char str1[6];
   sprintf(str1, "%d", dob.day);
   strcat(buffer, str1);
   strncat(buffer, &comma, 1);

   char str2[6];
   sprintf(str2, "%d", dob.month);
   strcat(buffer, str2);
   strncat(buffer, &comma, 1);

   char str3[6];
   sprintf(str3, "%d", dob.year);
   strcat(buffer, str3);

   char b=')';
   strncat(buffer, &b, 1);
   char s=';';
   strncat(buffer, &s, 1);


   char * errmsg=0;
   
   if (sqlite3_exec(db, buffer,0,0,&errmsg) == SQLITE_OK) {
      id = (_id_t)sqlite3_last_insert_rowid(db);
   }
   return id;
}

void storeStatus(sqlite3 *db, _id_t election, Status new_status) {
   sqlite3_stmt *stmt;
   const char *sql = "UPDATE Election SET status=? WHERE id=?";
   sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
   sqlite3_bind_int(stmt, 1, (int)new_status);
   sqlite3_bind_int(stmt, 2, election);
   sqlite3_step(stmt);
   sqlite3_finalize(stmt);
   if (new_status == PUBLISHED) {
      const char *totals = "UPDATE Candidate SET votes=(\
                              SELECT COUNT(*) FROM Vote WHERE\
                              Vote.candidate=Candidate.id AND\
                              Vote.office=Candidate.office)";
      sqlite3_prepare_v2(db, totals, -1, &stmt, NULL);
      sqlite3_step(stmt);
      sqlite3_finalize(stmt);
   }
}

void deleteElection(sqlite3 *db, _id_t election) {
   sqlite3_stmt *stmt;
   const char *sql = "DELETE FROM Election WHERE id=?";
   sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
   sqlite3_bind_int(stmt, 1, election);
   sqlite3_step(stmt);
   sqlite3_finalize(stmt);
}

void getVoter(sqlite3 *db, _id_t voter_id, Registration* dest) {
   sqlite3_stmt *stmt;
   const char *sql = "SELECT name,county,zip,dob_day,dob_mon,dob_year\
                      FROM Registration WHERE id=?";
   sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
   sqlite3_bind_int(stmt, 1, voter_id);
   sqlite3_step(stmt);
   strncpy(dest->name, (char *)sqlite3_column_text(stmt, 0), MAX_NAME_LEN-1);
   strncpy(dest->county, (char *)sqlite3_column_text(stmt, 1),MAX_NAME_LEN-1);
   (dest->name)[MAX_NAME_LEN-1] = '\0';
   (dest->county)[MAX_NAME_LEN-1] = '\0';
   dest->zip = sqlite3_column_int(stmt, 2);
   (dest->dob).day = sqlite3_column_int(stmt, 3);
   (dest->dob).month = sqlite3_column_int(stmt, 4);
   (dest->dob).year = sqlite3_column_int(stmt, 5);
   sqlite3_finalize(stmt);
}

void getElection(sqlite3 *db, _id_t election_id, Election* dest) {
   sqlite3_stmt *stmt;
   const char *sql = "SELECT deadline_day,deadline_mon,deadline_year,status\
                      FROM Election WHERE id=?";
   sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
   sqlite3_bind_int(stmt, 1, election_id);
   sqlite3_step(stmt);
   (dest->deadline).day = sqlite3_column_int(stmt, 0);
   (dest->deadline).month = sqlite3_column_int(stmt, 1);
   (dest->deadline).year = sqlite3_column_int(stmt, 2);
   dest->status = (Status)sqlite3_column_int(stmt, 3);
   sqlite3_finalize(stmt);
}

void storeVote(sqlite3 *db, _id_t voter, _id_t candidate, _id_t office) {
   sqlite3_stmt *stmt;
   const char *sql = "INSERT INTO Vote(voter,candidate,office)\
                      VALUES (?, ?, ?)";
   sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
   sqlite3_bind_int(stmt, 1, voter);
   sqlite3_bind_int(stmt, 2, candidate);
   sqlite3_bind_int(stmt, 3, office);
   sqlite3_step(stmt);
   sqlite3_finalize(stmt);
}

int getVote(sqlite3 *db, _id_t voter_id, _id_t office_id) {
   int count;
   sqlite3_stmt *stmt;
   const char *sql = "SELECT COUNT(*) FROM Vote WHERE\
                      voter=? AND office=?";
   sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
   sqlite3_bind_int(stmt, 1, voter_id);
   sqlite3_bind_int(stmt, 2, office_id);
   sqlite3_step(stmt);
   count = sqlite3_column_int(stmt, 0);
   sqlite3_finalize(stmt);
   return count;
}

void getVoters(sqlite3 *db) {
   sqlite3_stmt *stmt;
   const char *sql = "SELECT name,county,zip,dob_day,dob_mon,dob_year\
                      FROM Registration";
   sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
   printf("[\n");
   bool is_first = false;
   while (sqlite3_step(stmt) != SQLITE_DONE) {
      if (!is_first) {
         is_first = true;
      } else {
         printf(",\n");
      }
      printf("{\"name\": \"%s\", \"county\": \"%s\", \"zip\": \"%d\", ",
             sqlite3_column_text(stmt, 0),
             sqlite3_column_text(stmt, 1),
             sqlite3_column_int(stmt, 2));
      printf("\"dob\": \"%d-%02d-%02d\"}",
             sqlite3_column_int(stmt, 5)+1900,
             sqlite3_column_int(stmt, 4),
             sqlite3_column_int(stmt, 3));
   }
   printf("\n]\n");
   sqlite3_finalize(stmt);
}

void getElections(sqlite3 *db) {
   system("./database_helper.py"); /* U+1F914 */
}
