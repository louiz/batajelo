/** @addtogroup User
 *  @{
 */

/**
 * Contains all Friend Requests query
 * @class Request
 */

#include <iostream>
#include <string>

#ifndef __FRIEND_REQUEST_HPP__
# define __FRIEND_REQUEST_HPP__

#include <database/request.hpp>

class FriendRequest: public Request
{
public:
  FriendRequest();
  ~FriendRequest();
  void accept(const DbObject*, const DbObject*);
};

#endif
/**@}*/

