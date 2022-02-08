#include "player.hh"
#include <iostream>

Player::Player(const string name, int points):
    name_(name), points_(points)
{

}

bool Player::has_won()
{
    if (points_ == 50){
        return true;
    } else {
        return false;
    }
}

string Player::get_name()
{
    return name_;
}

int Player::get_points()
{
    return points_;
}

void Player::add_points(int points)
{
    if (points_ + points <= 50){
        points_ += points;
    } else {
        cout << name_ << " gets penalty points!" << endl;
        points_ = 25;
    }
}
