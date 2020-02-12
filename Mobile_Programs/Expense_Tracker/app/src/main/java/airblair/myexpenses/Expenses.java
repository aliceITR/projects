package airblair.myexpenses;

import androidx.room.ColumnInfo;
import androidx.room.Entity;
import androidx.room.Ignore;
import androidx.room.PrimaryKey;
import android.content.ContentValues;
import android.provider.BaseColumns;


@Entity(tableName = Expenses.TABLE_NAME)

public class Expenses {

    private static final int DATABASE_VERSION = 1;
    private static final String DATABASE_NAME = "expenseTracker.db";
    public static final String TABLE_NAME = "Expenses";
    public static final String COLUMN_ID = "entryID";
    public static final String COLUMN_NAME = "Name";
    public static final String COLUMN_CATEGORY = "Category";
    public static final String COLUMN_DATE = "Date";
    public static final String COLUMN_AMOUNT = "Amount";
    public static final String COLUMN_NOTE = "Note";

    @ColumnInfo(name = COLUMN_NAME)
    private String name;

    @ColumnInfo(name = COLUMN_CATEGORY)
    private String category;

    @ColumnInfo(name = COLUMN_DATE)
    private String date;

    @ColumnInfo(name = COLUMN_AMOUNT)
    private double amount;

    @ColumnInfo(name = COLUMN_NOTE)
    private String note;

    @PrimaryKey(autoGenerate = true)
    @ColumnInfo(index = true, name = COLUMN_ID)
    private long _id;




    @Ignore
    public Expenses(){}

//    public Expenses(String name, String category, String date, double amount, String note, long id)
//    {
//        this.name = name;
//        this.category = category;
//        this.date = date;
//        this.amount = amount;
//        this.note = note;
//        _id = id;
//    }
    public Expenses(String name, String category, String date, double amount, String note)
    {
        this.name = name;
        this.category = category;
        this.date = date;
        this.amount = amount;
        this.note = note;
    }

    public void setName(String name)
    {
        this.name = name;
    }

    public void setCategory(String category)
    {
        this.category = category;
    }
    public void setDate(String date)
    {
        this.date = date;
    }
    public void setName(double amount)
    {
        this.amount = amount;
    }
    public void setNote(String note)
    {
        this.note = note;
    }

    public void setId(long id)
    {
        _id = id;
    }

    public String getName()
    {
        return name;
    }

    public String getCategory()
    {
        return category;
    }

    public String getDate()
    {
        return date;
    }

    public double getAmount()
    {
        return amount;
    }

    public String getNote()
    {
        return note;
    }

    public long getId()
    {
        return _id;
    }

    public static Expenses fromContentValues(ContentValues values) {
        final Expenses expenseData = new Expenses();
        if (values.containsKey(COLUMN_ID)) {
            expenseData._id = values.getAsLong(COLUMN_ID);
        }
        if (values.containsKey(COLUMN_NAME)) {
            expenseData.name = values.getAsString(COLUMN_NAME);
        }
        if (values.containsKey(COLUMN_CATEGORY)) {
            expenseData.category = values.getAsString(COLUMN_CATEGORY);
        }
        if (values.containsKey(COLUMN_DATE)) {
            expenseData.date = values.getAsString(COLUMN_DATE);
        }
        if (values.containsKey(COLUMN_AMOUNT)) {
            expenseData.amount = values.getAsFloat(COLUMN_AMOUNT);
        }
        if (values.containsKey(COLUMN_NOTE)) {
            expenseData.note = values.getAsString(COLUMN_NOTE);
        }
        return expenseData;
    }
}
