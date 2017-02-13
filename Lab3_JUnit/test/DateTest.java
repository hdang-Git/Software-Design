import org.junit.jupiter.api.Test;

import java.util.Date;

import static org.junit.jupiter.api.Assertions.*;

/**
 * Created by Hai Dang on 2/13/2017.
 */
class DateTest {
    @Test
    void testYear(){
        DateImpl d1 = new DateImpl(2009, 12, 25);
        assertEquals(2009, d1.getYear());
    }

    @Test
    void testMonth(){
        DateImpl d1 = new DateImpl(2009, 12, 25);
        assertEquals(12, d1.getMonth());
    }

    @Test
    void testDay(){
        DateImpl d1 = new DateImpl(2009, 12, 25);
        assertEquals(25, d1.getDay());
    }

    @Test
    void getDateDifferenceForYears() {
        DateImpl d1 = new DateImpl(2009, 12, 25);
        DateImpl d2 = new DateImpl(2010, 12, 25);
        int yearToDays1 = d1.getYear() * 365;
        int yearToDays2 = d2.getYear() * 365;;
        int daysBetween = yearToDays2 - yearToDays1;
        assertEquals(daysBetween, d1.daysTo(d2));

    }

    @Test
    void getDateDifferenceForAll(){
        DateImpl d1 = new DateImpl(2009, 10, 20);
        DateImpl d2 = new DateImpl(2010, 12, 25);

        int daysBetween = 365+60+5;
        assertEquals(daysBetween, d1.daysTo(d2));
    }

    @Test
    void getDateDifferenceMultiple(){
        DateImpl d1 = new DateImpl(2009, 10, 20);
        DateImpl d2 = new DateImpl(2010, 12, 25);
        DateImpl d3 = new DateImpl(2011, 12, 25);

        assertEquals(365+60+5, d1.daysTo(d2));
        assertEquals(365*2+2*30+5, d1.daysTo(d3));
        assertEquals(365, d2.daysTo(d3));
    }


    @Test
    void getDateDifferenceNegative(){
        DateImpl d1 = new DateImpl(2010, 12, 25);
        DateImpl d2 = new DateImpl(2011, 12, 25);
        assertFalse(d2.daysTo(d1) < 0);
    }

    //This test is supposed to fail
    @Test
    void getDateDifferenceNegativeFail(){
        DateImpl d1 = new DateImpl(2010, 12, 25);
        DateImpl d2 = new DateImpl(2011, 12, 25);
        assertFalse(d2.daysTo(d1) > 0);
    }

    //This test is supposed to fail
    @Test
    void getDateDifferenceFalseValue(){

        DateImpl d1 = new DateImpl(2010, 12, 25);
        DateImpl d2 = new DateImpl(2011, 12, 25);
        assertEquals(5,d1.daysTo(d2));
    }

}