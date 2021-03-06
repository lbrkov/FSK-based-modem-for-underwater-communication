#include <NMEAGPS.h>
#include <GPSport.h>
#include <Streamers.h>

static NMEAGPS gps;

static gps_fix fix;

static void doSomeWork()
{

  trace_all(DEBUG_PORT, gps, fix);
}

static void GPSloop()
{
  while (gps.available(gpsPort))
  {
    fix = gps.read();
    doSomeWork();
  }
}
void setup()
{
  Serial2.begin(9600);
  Serial3.begin(9600);
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  DEBUG_PORT.begin(9600);
  while (!DEBUG_PORT)
    ;

  DEBUG_PORT.print(F("NMEA.INO: started\n"));
  DEBUG_PORT.print(F("  fix object size = "));
  DEBUG_PORT.println(sizeof(gps.fix()));
  DEBUG_PORT.print(F("  gps object size = "));
  DEBUG_PORT.println(sizeof(gps));
  DEBUG_PORT.println(F("Looking for GPS device on " GPS_PORT_NAME));

#ifndef NMEAGPS_RECOGNIZE_ALL
#error You must define NMEAGPS_RECOGNIZE_ALL in NMEAGPS_cfg.h!
#endif

#ifdef NMEAGPS_INTERRUPT_PROCESSING
#error You must *NOT* define NMEAGPS_INTERRUPT_PROCESSING in NMEAGPS_cfg.h!
#endif

#if !defined(NMEAGPS_PARSE_GGA) & !defined(NMEAGPS_PARSE_GLL) & \
    !defined(NMEAGPS_PARSE_GSA) & !defined(NMEAGPS_PARSE_GSV) & \
    !defined(NMEAGPS_PARSE_RMC) & !defined(NMEAGPS_PARSE_VTG) & \
    !defined(NMEAGPS_PARSE_ZDA) & !defined(NMEAGPS_PARSE_GST)

  DEBUG_PORT.println(F("\nWARNING: No NMEA sentences are enabled: no fix data will be displayed."));

#else
  if (gps.merging == NMEAGPS::NO_MERGING)
  {
    DEBUG_PORT.print(F("\nWARNING: displaying data from "));
    DEBUG_PORT.print(gps.string_for(LAST_SENTENCE_IN_INTERVAL));
    DEBUG_PORT.print(F(" sentences ONLY, and only if "));
    DEBUG_PORT.print(gps.string_for(LAST_SENTENCE_IN_INTERVAL));
    DEBUG_PORT.println(F(" is enabled.\n"
                         "  Other sentences may be parsed, but their data will not be displayed."));
  }
#endif

  DEBUG_PORT.print(F("\nGPS quiet time is assumed to begin after a "));
  DEBUG_PORT.print(gps.string_for(LAST_SENTENCE_IN_INTERVAL));
  DEBUG_PORT.println(F(" sentence is received.\n"
                       "  You should confirm this with NMEAorder.ino\n"));

  trace_header(DEBUG_PORT);
  DEBUG_PORT.flush();

  gpsPort.begin(9600);
}

//--------------------------

void loop()
{
  GPSloop();
  while (Serial2.available())
  {
    Serial3.println(Serial2.readString());
    // char c = Serial3.read();  //gets one byte from serial buffer
    delay(100); // slow looping to allow buffer to fill with next character
                //  send data only when you receive data:
  }
}
