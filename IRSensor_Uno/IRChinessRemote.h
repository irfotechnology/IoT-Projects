
// IR Minmum and Maximum values range
#define IR_REPEAT 0xFFFFFFFF
#define IR_MIN_VAL 0xF70000
#define IR_MAX_VAL 0xF7FFFF
//Buttons definations

//------------------------------------------------------------------------------
// Mark & Space matching functions
//

class IRChinessRemote
{
public:
  // Constructor(s)
  IRChinessRemote() {}
  virtual ~IRChinessRemote() {}

  /*! @brief Get info about the sensor itself */
  char *FIND_KEY(unsigned long data);
};
