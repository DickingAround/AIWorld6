#ifndef predator_test_c
#define predator_test_c
//We'll take the world and add some predators to it. And then run it.
//So all this has to do is append the world with some predators

void add_man_made_predators() {
 //Always emit a signature 101 to know it's you.
 //If a location has 101 and it's near you and your health is > 100, replicate
 //If there is no agent around or all agents have 101, grow
 //If there's an agent near you, pursue it
 //If there's an agent directly in front of you attack it
 // CONNECTIONS
 //ATTACK
 //energy[12] > 1 > mid1
 //sig0[12] > 1 > mid2
 //sig1[12] > -2 > mid3
 //sig2[12] > 1 > mid4
 //mid1 > 1 > attack
 //mid2 > 1 > attack
 //mid3 > 1 > attack
 //mid4 > 1 > attack
 
}
#endif
