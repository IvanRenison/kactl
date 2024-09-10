/**
 * Author: Iván Renison
 * Date: 2024-08-29
 * License: CC0
 * Source: notebook el vasito
 * Description: Get the day of the week for a given date.
 * Days and months start from 1 and days of the week are returned starting on Sunday from 0.
 * Time: O(1)
 * Status: problem tested
 */
#pragma once

ll DayOfWeek(ll y, ll m, ll d) {
	vi ttt = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
	y -= m < 3;
	return (y + y/4 - y/100 + y/400 + ttt[m-1] + d) % 7;
}
