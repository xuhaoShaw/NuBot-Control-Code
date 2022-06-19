#ifndef _ROS_SERVICE_WriteState_h
#define _ROS_SERVICE_WriteState_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "cartographer_ros_msgs/StatusResponse.h"

namespace cartographer_ros_msgs
{

static const char WRITESTATE[] = "cartographer_ros_msgs/WriteState";

  class WriteStateRequest : public ros::Msg
  {
    public:
      typedef const char* _filename_type;
      _filename_type filename;

    WriteStateRequest():
      filename("")
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      uint32_t length_filename = strlen(this->filename);
      varToArr(outbuffer + offset, length_filename);
      offset += 4;
      memcpy(outbuffer + offset, this->filename, length_filename);
      offset += length_filename;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint32_t length_filename;
      arrToVar(length_filename, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_filename; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_filename-1]=0;
      this->filename = (char *)(inbuffer + offset-1);
      offset += length_filename;
     return offset;
    }

    const char * getType(){ return WRITESTATE; };
    const char * getMD5(){ return "030824f52a0628ead956fb9d67e66ae9"; };

  };

  class WriteStateResponse : public ros::Msg
  {
    public:
      typedef cartographer_ros_msgs::StatusResponse _status_type;
      _status_type status;

    WriteStateResponse():
      status()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->status.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->status.deserialize(inbuffer + offset);
     return offset;
    }

    const char * getType(){ return WRITESTATE; };
    const char * getMD5(){ return "4e6ca4e44081fa06b258fa12804ea7cb"; };

  };

  class WriteState {
    public:
    typedef WriteStateRequest Request;
    typedef WriteStateResponse Response;
  };

}
#endif
