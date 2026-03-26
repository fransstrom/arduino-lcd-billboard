/**
 * @file billboard.h
 * @brief Billboard system public interface
 *
 * Controls the advertising billboard that cycles through company ads
 * with weighted random selection based on advertising balance.
 */

#ifndef BILLBOARD_H
#define BILLBOARD_H

#include "stdbool.h"

struct Billboard;
struct Company;

/**
 * @brief Main entry point - runs the billboard indefinitely
 */
void billboard_run(void);

/**
 * @brief Adds a company to the billboard
 * @param billboard Target billboard
 * @param company   Company to add
 * @return true on success, false on failure
 */
bool billboard_add_company(struct Billboard *billboard,
                           struct Company *company);

/**
 * @brief Removes a company from the billboard
 * @param billboard Target billboard
 * @param company   Company to remove
 * @return true if found and removed, false otherwise
 */
bool billboard_remove_company(struct Billboard *billboard,
                              struct Company *company);

/**
 * @brief Initializes billboard with demo companies and ads
 * @param billboard Billboard to initialize
 */
void billboard_prep(struct Billboard *billboard);

#endif /* BILLBOARD_H */
