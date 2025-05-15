class Point {
  /**
   * @param {*} who_served 1 or 2. Indicates if player 1 or player 2 served the point.
   * @param {*} who_won 1 or 2. Indicates if player 1 or player 2 won the point.
   * @param {*} point_number The number point that it is in the context of the game. First point is 1, second point is 2, etc.
   * @param {*} score_after_point The score after the point was played. An array of two numbers. The first number is the score of player 1, the second number is the score of player 2. 
   */
  constructor(who_served, who_won, point_number, score_after_point) {
    this.who_served = who_served;
    this.who_won = who_won;
    this.point_number = point_number;
    this.score_after_point = score_after_point;
  }
}

export default Point;