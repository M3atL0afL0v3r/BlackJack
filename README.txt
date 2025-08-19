BlackJack

This is a console-based implementation of Blackjack (also known as 21). The game supports multiple players (1–7), betting, dealer logic, and special moves like splitting pairs. It demonstrates C++ fundamentals such as structs, vectors, input validation, random shuffling, and basic game state management. 

⚙️ Features

🃏 Standard 52-card deck with suits and ranks.

👥 Supports 1–7 players.

💵 Betting system (players start with $1000).

🎯 Hand evaluation (automatic ace handling for soft/hard values).

🤝 Split option when dealt a pair.

🏦 Dealer follows Blackjack rules (hits until 17 or higher).

🏆 Final results with money adjustments (win, push, or loss).

🔄 Continuous game loop structure (easily expandable).

🎮 How to Play

Program asks for number of players (1–7).

Each player starts with $1000.

Each round:

	Players place bets.

	Two cards are dealt to each player and the dealer.

	Players may Hit, Stay, or Split (if pair).

	Dealer reveals hand and plays by Blackjack rules.

	Winners are announced, and balances updated.

🖥️ Running the Game

Open X64/Release/ and rune the .exe

📁 Project Structure
BlackJack/
│── BlackJack/
│   └── main.cpp     	   # Program code
│   └── x64/    	   # Debug and Release executables
│── LICENSE.txt
│── README.txt

✨ Possible Improvements

Add insurance and double down options.

Allow multiple rounds until players choose to quit.

Save/load player balances between sessions.

Create a graphical interface (SFML, SDL, or similar).

📜 License

This project is free to use, modify, and learn from
