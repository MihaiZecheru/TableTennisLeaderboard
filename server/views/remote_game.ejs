<!DOCTYPE html>
<html>
<head>
  <title>Table Tennis</title>
  <link rel="stylesheet" href="/index.css">
  <link rel="icon" href="/table_tennis_racket.png" type="image/x-icon">
  <link href="https://fonts.googleapis.com/css2?family=Comfortaa&display=swap" rel="stylesheet">
  <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.5.0/css/all.min.css">
</head>
<body>

  <div class="live-score-display-container">

    <div class="live-score-p1-display">
      <span class="live-score-p1-score"></span>
    </div>
    <div class="live-score-p2-display">
      <span class="live-score-p2-score"></span>
    </div>

  </div>

  <style>
    .live-score-p1-display,
    .live-score-p2-display {
      user-select: none;
      cursor: pointer;
    }
  </style>

  <script>
    let p1_id;
    let p2_id;
    let p1_score = 0;
    let p2_score = 0;
    let point_count = 0;
    let server = -1; // will be set to either 1 or 2 when the game starts
    let set_length = -1; // will be set to either 6, 11, or 21 when the game starts
    const point_history = [];

    function _change_server() {
      if (server === 1) server = 2;
      else server = 1;
    }

    function in_deuce() {
      return (set_length == 21 && p1_score >= 20 && p2_score >= 20) || (set_length == 11 && p1_score >= 10 && p2_score >= 10);
    }

    function change_server_if_necessary()
    {
      if (in_deuce() || set_length == 6) { // Server changes every point during deuce and when playing first to 6
        _change_server();
      } else if (set_length == 21 && point_count % 5 == 0) {
        _change_server();
      } else if (set_length == 11 && point_count % 2 == 0) {
        _change_server();
      }
    }

    function getUTC() {
      const now = new Date();

      const year = now.getUTCFullYear();
      const month = String(now.getUTCMonth() + 1).padStart(2, "0");
      const day = String(now.getUTCDate()).padStart(2, "0");
      const hour = String(now.getUTCHours()).padStart(2, "0");
      const minute = String(now.getUTCMinutes()).padStart(2, "0");
      const second = String(now.getUTCSeconds()).padStart(2, "0");

      return `${year}-${month}-${day} ${hour}:${minute}:${second}`;
    }

    const started_at = getUTC();

    function push_game_results(p1_id, p2_id, set_length, started_at) {
      const data = { p1_id, p2_id, set_length, point_history, started_at, ended_at: getUTC() };
      console.log(data)
      fetch('/api/upload-set', {
        method: 'POST',
        headers: {
          "Content-Type": "application/json"
        },
        body: JSON.stringify(data)
      }).then((res) => {
        if (res.status == 200) {
          alert("Game data submitted successfully");
        } else {
          alert("ERROR: upload failed");
        }
        window.location.reload();
      }).catch((err) => {
        console.error(err);
        alert("There was a problem submitting the game to the server. The data object has been logged to the console");
        console.log("DATA OBJECT: ", data);
        window.location.reload();
      });
    }

    function check_end_game() {
      let game_over = false;

      if (set_length == 6 && !in_deuce() && (p1_score == 6 || p2_score == 6)) {
        game_over = true;
      } else if (set_length == 11 && !in_deuce() && (p1_score == 11 || p2_score == 11)) {
        game_over = true;
      } else if (set_length == 21 && !in_deuce() && (p1_score == 21 || p2_score == 21)) {
        game_over = true;
      }

      if (game_over) {
        push_game_results(p1_id, p2_id, set_length, started_at);
      }
    }

    document.querySelector('.live-score-p1-display')
      .addEventListener('click', () => {
        p1_score++;
        point_count++;
        point_history.push({ s: server, w: 1 });
        change_server_if_necessary();
        update_p1_score_display();
        update_p2_score_display();
        check_end_game();
      });

    document.querySelector('.live-score-p2-display')
      .addEventListener('click', () => {
        p2_score++;
        point_count++;
        point_history.push({ s: server, w: 2 });
        change_server_if_necessary();
        update_p1_score_display();
        update_p2_score_display();
        check_end_game();
      });

    function title_case_word(word) {
      if (!word) return null;
      return word[0].toUpperCase() + word.slice(1).toLowerCase();
    }

    function update_p1_score_display() {
      document.querySelector('.live-score-p1-score').innerText = p1_score + (server == 1 ? '•' : '');
    }

    function update_p2_score_display() {
      document.querySelector('.live-score-p2-score').innerText = p2_score + (server == 2 ? '•' : '');
    }

    async function start_game() {
      const p1_name = title_case_word((window.prompt("Enter player one's name") || "").trim().split(" ")[0]);
      if (!p1_name) return null;
      p1_id = await get_player_id_from_name(p1_name);

      if (!p1_id) {
        alert("Could not find a player with that name");
        return null;
      }

      const p2_name = title_case_word((window.prompt("Enter player two's name") || "").trim().split(" ")[0]);
      if (!p2_name) return null;
      p2_id = await get_player_id_from_name(p2_name);

      if (!p2_id) {
        alert("Could not find a player with that name");
        return null;
      }

      if (p1_id === p2_id) {
        alert("The two players cannot be the same");
        return null;
      }

      const set_length_response = (window.prompt("Will this set be played to 6, 11, or 21?") || "").trim();
      if (set_length_response == 6) {
        set_length = 6;
      } else if (set_length_response == 21) {
        set_length = 21;
      } else {
        set_length = 11;
      }

      const name_of_first_server = title_case_word((window.prompt("Who should serve first?") || "").trim().split(" ")[0]);
      if (name_of_first_server == 1 || name_of_first_server == 2) {
        server = parseInt(name_of_first_server);
      } else if (name_of_first_server == p1_name) {
        server = 1;
      } else if (name_of_first_server == p2_name) {
        server = 2;
      } else {
        alert("That player is not in the game");
        return null;
      }

      update_p1_score_display();
      update_p2_score_display();
      window.title = `${p1_name} vs ${p2_name}`;

      return { p1: { name: p1_name, id: p1_id }, p2: { name: p2_name, id: p2_id }};
    }

    async function get_player_id_from_name(name) {
      const player_id = await fetch("/api/player-id/" + name).then((res) => res.text());
      if (player_id.startsWith("Error fetching player"))
        return null;
      return player_id;
    }

    window.onload = setTimeout(async () => {
      const players = await start_game();
      if (!players) return window.location.reload();
      const player1 = players.p1;
      const player2 = players.p2;
    }, 500);
  </script>

</body>
</html>
