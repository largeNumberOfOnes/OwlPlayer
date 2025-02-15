#include "player.h"
#include "abstract.h"

#include <iostream>
#include <string>



void Player::load(std::string file_path) {

}

void Player::play() {
    std::cout << "Start playing" << std::endl;
}

void Player::forward(seconds step) {
    std::cout << "Step forward on " << step << " millisecond" << std::endl;
}

void Player::backward(seconds step) {
    std::cout << "Step backward on" << step << " millisecond" << std::endl;
}

void Player::set_volume(volume vol) {
    std::cout << "Set volume " << vol << std::endl;
}

volume Player::get_volume() const {
    std::cout << "Get volume " << vol << std::endl;
    return vol;
}
