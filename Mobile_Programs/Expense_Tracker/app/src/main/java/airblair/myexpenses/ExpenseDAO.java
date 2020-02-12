package airblair.myexpenses;

import androidx.lifecycle.LiveData;
import androidx.room.Dao;
import androidx.room.Delete;
import androidx.room.Insert;
import androidx.room.OnConflictStrategy;
import androidx.room.Query;
import androidx.room.Update;

import java.util.List;

@Dao
public interface ExpenseDAO {
    @Query("SELECT * FROM " + Expenses.TABLE_NAME)
    LiveData<List<Expenses>> selectAll();


    @Query("SELECT * FROM "+Expenses.TABLE_NAME +  " ORDER BY " +Expenses.COLUMN_NAME+" ASC")
    LiveData<List<Expenses>> selectByName();

    /**
     * Select by the ID.
     *
     * @param id The row ID.
     * @return A {@link LiveData <List<Score>>>} of the selected score.
     */
    @Query("SELECT * FROM " + Expenses.TABLE_NAME + " WHERE " + Expenses.COLUMN_ID + " = :id")
    LiveData<List<Expenses>> selectById(long id);


    @Insert(onConflict = OnConflictStrategy.REPLACE)
    void insertAll(List<Expenses> scores);


    @Query("SELECT COUNT(*) FROM " + Expenses.TABLE_NAME)
    int count();

    /**
     * Inserts a score into the table.
     *
     * @param expenseData A new person and score.
     * @return The row ID of the newly inserted data.
     */
    @Insert
    long insert(Expenses expenseData);

    /**
     * Inserts multiple scores into the database
     *
     * @param expenseDatas An array of new score data.
     * @return The row IDs of the newly inserted scores.
     */
    @Insert
    long[] insertAll(Expenses[] expenseDatas);

    @Delete
    void delete(Expenses expense);
    /**
     * Delete a score by the ID.
     *
     * @param id The row ID.
     * @return A number of scores deleted. This should always be {@code 1}.
     */
    @Query("DELETE FROM " + Expenses.TABLE_NAME + " WHERE " + Expenses.COLUMN_ID + " = :id")
    int deleteById(long id);

    /**
     * Delete all scores.
     */
    @Query("DELETE FROM " + Expenses.TABLE_NAME)
    void deleteAll();


    /**
     * Update the score. The score is identified by the row ID.
     *
     * @param expenseData The score to update.
     * @return A number of scores updated. This should always be {@code 1}.
     */
    @Update
    int update(Expenses expenseData);


}
