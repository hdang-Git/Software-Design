import java.util.Calendar;
import java.util.Locale;

/**
 * Created by Hai Dang on 2/13/2017.
 */
public class DateImpl {


    private int year;
    private int month;
    private int day;


    public DateImpl(int year, int month, int day){
        this.year = year;
        this.month = month;
        this.day = day;
    }

    public int getYear() {
        return year;
    }

    public void setYear(int year) {
        this.year = year;
    }

    public int getMonth() {
        return month;
    }

    public void setMonth(int month) {
        this.month = month;
    }

    public int getDay() {
        return day;
    }

    public void setDay(int day) {
        this.day = day;
    }

    //years = 365
    //days = 30
    public int daysTo(DateImpl d2){
        int yearDif = (d2.getYear() - this.year)* 365;
        int monthDif = (d2.getMonth() - this.month) * 30;
        int dayDif = d2.getDay() - this.day;
        return Math.abs(yearDif + monthDif + dayDif);
    }
}
