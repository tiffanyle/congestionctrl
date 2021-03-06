#include <iostream>
#include <unordered_map>
#include <string>
#include "controller.hh"
#include "timestamp.hh"

using namespace std;

float the_window_size = 50;
float min_wind = 1.0;
uint64_t last_seq_ack_seen = 0;
uint64_t last_time_sent = 0;

/* Default constructor */
Controller::Controller( const bool debug )
  : debug_( debug )
{}

/* Get current window size, in datagrams */
unsigned int Controller::window_size( void )
{
  /* Default: fixed window size of 100 outstanding datagrams */

     
  if ( debug_ ) {
    cerr << "At time " << timestamp_ms()
	 << " window size is " << (unsigned int) the_window_size << endl;
  } 
 
  return (unsigned int) the_window_size;
}

/* A datagram was sent */
void Controller::datagram_was_sent( const uint64_t sequence_number,
				    /* of the sent datagram */
				    const uint64_t send_timestamp )
                                    /* in milliseconds */
{
  /* Default: take no action */

  if ( debug_ ) {
    cerr << "At time " << send_timestamp
	 << " sent datagram " << sequence_number << endl;
  }
 
}

/* An ack was received */
void Controller::ack_received( const uint64_t sequence_number_acked,
			       /* what sequence number was acknowledged */
			       const uint64_t send_timestamp_acked,
			       /* when the acknowledged datagram was sent (sender's clock) */
			       const uint64_t recv_timestamp_acked,
			       /* when the acknowledged datagram was received (receiver's clock)*/
			       const uint64_t timestamp_ack_received )
                               /* when the ack was received (by sender) */
{
  /* Default: take no action */
  
  if(sequence_number_acked>last_seq_ack_seen && timestamp_ack_received-send_timestamp_acked < timeout_ms()) { 
      	the_window_size = the_window_size+float(1/the_window_size);
  }
else{
	the_window_size =std::max(the_window_size/float(2),min_wind);
}
 last_seq_ack_seen = sequence_number_acked;
 
  if ( debug_ ) {
    cerr << "At time " << timestamp_ack_received
	 << " received ack for datagram " << sequence_number_acked
	 << " (send @ time " << send_timestamp_acked
	 << ", received @ time " << recv_timestamp_acked << " by receiver's clock)"
	 << endl;
  }
}

/* How long to wait (in milliseconds) if there are no acks
   before sending one more datagram */
unsigned int Controller::timeout_ms( void )
{
  return 1000; /* timeout of one second */
}
