class Game {
  /**
   * @param {number} id 
   * @param {number} p1_id 
   * @param {number} p2_id 
   * @param {number} winner_id 
   * @param {number} p1_score 
   * @param {number} p2_score 
   * @param {number} set_length 
   * @param {Point[]} point_history 
   * @param {Date} started_at 
   * @param {Date} ended_at 
   */
  constructor(id, p1_id, p2_id, winner_id, p1_score, p2_score, set_length, point_history, started_at, ended_at) {
    this.id = id;
    this.p1_id = p1_id;
    this.p2_id = p2_id;
    this.winner_id = winner_id;
    this.p1_score = p1_score;
    this.p2_score = p2_score;
    this.set_length = set_length;
    this.point_history = point_history; // Array of objects with 's' and 'w' properties
    this.started_at = started_at; // Date string or timestamp
    this.ended_at = ended_at; // Date string or timestamp
    
    this.match_length = (new Date(this.ended_at).getTime() - new Date(this.started_at).getTime()) / 1000; // Duration of the game in seconds
  }
}

export default Game;