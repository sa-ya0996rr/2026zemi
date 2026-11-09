#include "Player.h"
#include <tchar.h>

Player::Player() : ticket(false) {}
void Player::Reset() { ticket = false; }
bool Player::HasTicket() const { return ticket; }
void Player::SetTicket(bool value) { ticket = value; }
bool Player::UseTicket() { if (!ticket) return false; ticket = false; return true; }
