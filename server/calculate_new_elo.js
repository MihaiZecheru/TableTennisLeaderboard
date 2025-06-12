function determine_winner(point_history) {
  let p1_points = 0;
  let p2_points = 0;

  for (const point of point_history) {
    if (point.w === 1) p1_points++;
    else if (point.w === 2) p2_points++;
  }

  if (p1_points > p2_points) return 'p1';
  else if (p2_points > p1_points) return 'p2';
  else throw new Error('Invalid point_history');
}

/**
 * @param {*} winner_rating The rating of the winner before the match
 * @param {*} loser_rating The rating of the loser before the match
 * @param {*} k The K-factor, which determines volatile the elo change is. Default is 32.
 * @returns object with new_winner_rating and new_loser_rating
 */
function _calculate_new_elo_helper(winner_rating, loser_rating, k = 32) {
  const expected_winner = 1 / (1 + 10 ** ((loser_rating - winner_rating) / 400));
  const expected_loser = 1 - expected_winner;

  const new_winner_rating = Math.round(winner_rating + k * (1 - expected_winner));
  const new_loser_rating = Math.round(loser_rating + k * (0 - expected_loser));

  return { new_winner_rating, new_loser_rating };
}

/**
 * Calculate both p1 and p2's new ELO ratings based on the outcome of the match and their ELO ratings before the match.
 * @param {number} p1_elo_before The ELO rating of player 1 before the match
 * @param {number} p2_elo_before The ELO rating of player 2 before the match
 * @param {number} set_length The length of the set (6, 11, or 21). Used to calculate the K-factor.
 * @param {*} point_history Used to determine the winner of the match
 * @returns {{ p1_elo_change, p2_elo_change }} dictionary containing the change in ELO rating for each player (ie: -5, +7, etc)
 */
export function CalculateNewElo(p1_elo_before, p2_elo_before, set_length, point_history) {
  // Calculate the new ELO ratings for each of the players as a result of the match
  const winner = determine_winner(point_history); // string 'p1' or 'p2'

  // Determine ELO multiplier (k) based on set length
  // When playing to 21, k=32. When playing to 11, k=24. When playing to 6, k=16.
  let k;
  if (set_length === 6) {
    k = 16; // 0.5 of default K-factor for short sets (short sets are more volatile)
  } else if (set_length === 11) {
    k = 24; // 0.75 of default K-factor for medium sets
  } else if (set_length === 21) {
    k = 32; // Default K-factor for longer sets
  } else {
    throw new Error(`Invalid set_length: ${set_length}. Must be one of 6, 11, or 21.`);
  }

  let new_p1_elo, new_p2_elo;
  if (winner === 'p1') {
    const { new_winner_rating, new_loser_rating } = _calculate_new_elo_helper(p1_elo_before, p2_elo_before, k);
    new_p1_elo = new_winner_rating;
    new_p2_elo = new_loser_rating;
  } else if (winner === 'p2') {
    const { new_winner_rating, new_loser_rating } = _calculate_new_elo_helper(p2_elo_before, p1_elo_before, k);
    new_p1_elo = new_loser_rating;
    new_p2_elo = new_winner_rating;
  } else {
    throw new Error(`Invalid winner: ${winner}. Must be one of 'p1' or 'p2'.`);
  }

  const p1_elo_change = new_p1_elo - p1_elo_before;
  const p2_elo_change = new_p2_elo - p2_elo_before;
  return { p1_elo_change, p2_elo_change };
}
