


void checkGravityAndWins(){
    for (int c=0; c < circles.size(); c++){       
        
        int areas[4] = [-1,1,0,-7]; int points=0;
        for (int j=0; j < 4; j++)}{
            for (int i=c; i < circles.size(); c += 7+areas[j]){
                    if (cirColors.at(c).r+cirColors.at(c).g == 506) points++;   
            }
        } if (points == 3){Winner = "Yellow"; won=true;}

        points=0;
        for (int j=0; j < 4; j++)}{
            for (int i=c; i < circles.size(); c += 7+areas[j]){
                    if (cirColors.at(c).r+cirColors.at(c).g == 255) points++;   
            }
        } if (points == 3){Winner = "Red"; won = true;}

        
        
        
        points = 0
        if circles[c].color == [255,0,0]:
            points = 1
            if int(c.replace('circle',''))+1 not in [6,13,20,27,34,41] and int(c.replace('circle',''))+1 <41 and int(c.replace('circle','')) and circles[f"circle{int(c.replace('circle',''))+1}"].color == [255,0,0]:
                points += 1
                if int(c.replace('circle',''))+2 not in [6,13,20,27,34,41] and circles[f"circle{int(c.replace('circle',''))+2}"].color == [255,0,0]:
                    points += 1
                    if int(c.replace('circle',''))+3 not in [6,13,20,27,34,41] and circles[f"circle{int(c.replace('circle',''))+3}"].color == [255,0,0]:
                        points += 1
                        winner = 'Red'
                        win = 1
 
        points = 0
        if circles[c].color == [255,251,0]:
            points = 1
            if int(c.replace('circle',''))+1 not in [6,13,20,27,34,41] and int(c.replace('circle',''))+1 <41 and int(c.replace('circle',''))<42 and circles[f"circle{int(c.replace('circle',''))+1}"].color == [255,251,0]:
                points += 1
                if int(c.replace('circle',''))+2 not in [6,13,20,27,34,41] and circles[f"circle{int(c.replace('circle',''))+2}"].color == [255,251,0]:
                    points += 1
                    if int(c.replace('circle',''))+3 not in [6,13,20,27,34,41] and circles[f"circle{int(c.replace('circle',''))+3}"].color == [255,251,0]:
                        points += 1
                        winner = 'Yellow'
                        win = 1



        points = 0
        if circles[c].color == [255,0,0]:
            points = 1
            if int(c.replace('circle',''))-7 >= 0 and circles[f"circle{int(c.replace('circle',''))-7}"].color == [255,0,0]:
                points += 1
                if int(c.replace('circle',''))-14 >= 0 and circles[f"circle{int(c.replace('circle',''))-14}"].color == [255,0,0]:
                    points += 1
                    if int(c.replace('circle',''))-21 >= 0 and circles[f"circle{int(c.replace('circle',''))-21}"].color == [255,0,0]:
                        points += 1
                        winner = 'Red'
                        win = 1


        points = 0
        if circles[c].color == [255,251,0]:
            points = 1
            if int(c.replace('circle',''))-7 >= 0 and circles[f"circle{int(c.replace('circle',''))-7}"].color == [255,251,0]:
                points += 1
                if int(c.replace('circle',''))-14 >= 0 and circles[f"circle{int(c.replace('circle',''))-14}"].color == [255,251,0]:
                    points += 1
                    if int(c.replace('circle',''))-21 >= 0 and circles[f"circle{int(c.replace('circle',''))-21}"].color == [255,251,0]:
                        points += 1
                        winner = 'Yellow'
                        win = 1

        points = 0
        if circles[c].color == [255,0,0]:
            points = 1
            if int(c.replace('circle',''))-8 >= 0 and circles[f"circle{int(c.replace('circle',''))-8}"].color == [255,0,0]:
                points += 1
                if int(c.replace('circle',''))-16 >= 0 and circles[f"circle{int(c.replace('circle',''))-16}"].color == [255,0,0]:
                    points += 1
                    if int(c.replace('circle',''))-24 >= 0 and circles[f"circle{int(c.replace('circle',''))-24}"].color == [255,0,0]:
                        points += 1
                        winner = 'Red'
                        win = 1

        points = 0
        if circles[c].color == [255,251,0]:
            points = 1
            if int(c.replace('circle',''))-8 >= 0 and circles[f"circle{int(c.replace('circle',''))-8}"].color == [255,251,0]:
                points += 1
                if int(c.replace('circle',''))-16 >= 0 and circles[f"circle{int(c.replace('circle',''))-16}"].color == [255,251,0]:
                    points += 1
                    if int(c.replace('circle',''))-24 >= 0 and circles[f"circle{int(c.replace('circle',''))-24}"].color == [255,251,0]:
                        points += 1
                        winner = 'Yellow'
                        win = 1
        points = 0
        if circles[c].color == [255,0,0]:
            points = 1
            if int(c.replace('circle',''))-6 >= 0 and circles[f"circle{int(c.replace('circle',''))-6}"].color == [255,0,0]:
                points += 1
                if int(c.replace('circle',''))-12 >= 0 and circles[f"circle{int(c.replace('circle',''))-12}"].color == [255,0,0]:
                    points += 1
                    if int(c.replace('circle',''))-18 >= 0 and circles[f"circle{int(c.replace('circle',''))-18}"].color == [255,0,0]:
                        points += 1
                        winner = 'Red'
                        win = 1

        points = 0
        if circles[c].color == [255,251,0]:
            points = 1
            if int(c.replace('circle',''))-6 >= 0 and circles[f"circle{int(c.replace('circle',''))-6}"].color == [255,251,0]:
                points += 1
                if int(c.replace('circle',''))-12 >= 0 and circles[f"circle{int(c.replace('circle',''))-12}"].color == [255,251,0]:
                    points += 1
                    if int(c.replace('circle',''))-18 >= 0 and circles[f"circle{int(c.replacae('circle',''))-18}"].color == [255,251,0]:
                        points += 1
                        winner = 'Yellow'
                        win = 1
    }
}


