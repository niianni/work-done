/**
 *
 * @author Niina Ahola
 */
import java.time.LocalDate;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.Collections;
import java.time.DateTimeException;
import java.time.temporal.ChronoUnit;

public class Dates {
    
    public static class DateDiff {
        private String start; //muotoa YYYY-MM-DD
        private String end;   //muotoa YYYY-MM-DD
        private int diff;
        
        //rakennin
        private DateDiff(String start, String end, int diff) {
            this.start = start;
            this.end = end;
            this.diff = diff;
        }
        
        public String getStart() {
            return this.start;
        }

        public String getEnd() {
            return this.end;
        }

        public int getDiff() {
            return this.diff;
        }
        
        public String toString() {
            // palauttaa merkkijonon:
            // Friday 01.01.1999 --> Monday 01.01.2001: 731 days
            String start = this.getStart();
            String end = this.getEnd();
            int diff = this.getDiff();
            
            String sDay = start.split("\\-")[2];
            String sMonth = start.split("\\-")[1];
            String sYear = start.split("\\-")[0];
            
            String eDay = end.split("\\-")[2];
            String eMonth = end.split("\\-")[1];
            String eYear = end.split("\\-")[0];
            
            DateTimeFormatter formatter = DateTimeFormatter.ofPattern("EEEE dd.MM.yyyy");
            
            LocalDate startDay = LocalDate.of(Integer.parseInt(sYear), 
                    Integer.parseInt(sMonth), Integer.parseInt(sDay));
            
            LocalDate endDay = LocalDate.of(Integer.parseInt(eYear), 
                    Integer.parseInt(eMonth), Integer.parseInt(eDay));
            
            String sText = startDay.format(formatter);
            String eText = endDay.format(formatter);
            
            String toReturn = sText +" --> "+eText+ ": "+diff+" days";
            return toReturn;
        }   
    }
    
    public static DateDiff[] dateDiffs(String ...dateStrs) {
        //palauttaa saamiensa päivämäärien väliset erotukset
        //kuvaavat DateDiff-oliot taulukkona
        ArrayList<LocalDate> dates = new ArrayList<>();

        //käy dateStrs läpi 
        for (String param : dateStrs){
            String[] firstSplitParam = param.split("\\-");
            String[] secondSplitParam = param.split("\\.");            
            int year = 0;
            int month = 0;
            int day = 0;
            if (firstSplitParam.length == 3 && firstSplitParam[1].length() == 2 
                    && firstSplitParam[2].length() == 2 
                    && firstSplitParam[0].length() == 4){
                year = Integer.parseInt(firstSplitParam[0]);
                month = Integer.parseInt(firstSplitParam[1]);
                day = Integer.parseInt(firstSplitParam[2]);
            } else if (secondSplitParam.length == 3 && secondSplitParam[2].length() == 4) {
                year = Integer.parseInt(secondSplitParam[2]);
                month = Integer.parseInt(secondSplitParam[1]);
                day = Integer.parseInt(secondSplitParam[0]);    
            } 
            try {
                //ota talteen lailliset päivämäärät dates-listalle
                LocalDate date = LocalDate.of(year, month, day);
                dates.add(date);
            }
            catch (DateTimeException e) {
                System.out.format("The date \"%s\" is illegal!%n", param);
            }
        }
        //laske montako laillista päivämäärää on ja alusta palautettava lista
        var legalDates = new DateDiff[dates.size()-1];

        //lajittele päivämäärät aikajärjestykseen
        Collections.sort(dates, (LocalDate a, LocalDate b) -> {
            if (a.isBefore(b)){
                return -1; 
            } else if (b.isBefore(a)){
                return 1;
            } else {
                return 0;
            }
        });
        
        long diffLong = 0;
        int diffInt = 0;
        //käy järjestetty lista läpi
        //palauta kahta peräkkäistä päivämäärää kohden yksi DateDiff-olio
        for (int i = 0; i < dates.size()-1; ++i){
            diffLong = ChronoUnit.DAYS.between(dates.get(i), dates.get(i+1));
            diffInt = (int)diffLong;
            legalDates[i] = new DateDiff(dates.get(i).toString(), 
                    dates.get(i+1).toString(), diffInt);
        }
        return legalDates;
    }
}
