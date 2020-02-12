package airblair.myexpenses;

import androidx.annotation.NonNull;
import androidx.lifecycle.Observer;

import android.content.Intent;
import android.os.Bundle;
import androidx.annotation.Nullable;
import com.google.android.material.floatingactionbutton.FloatingActionButton;
import com.google.android.material.snackbar.Snackbar;
import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.DefaultItemAnimator;
import androidx.recyclerview.widget.ItemTouchHelper;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;
import android.util.Log;
import android.view.View;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity implements Dialog.DialogListener, deleteDialog.DialogListener, UpdateDialog.DialogListener{
    RecyclerView recycle;
    //final myAdapter mAdapter;
    String TAG = "MainActivity";
    AppDatabase ad;

//So i struggled with update and i decided to brute force it
    public static String Uname = "";
    public static String Ucat = "";
    public static String Udate= "";
    public static String Uprice= "";
    public static String Unote= "";
    public static String UID= "";

    @Override
    public void applyTexts(final String typedname,final String typedCategory, final String typedDate, final double typedAmount,final  String typedNote)
    {

        Thread myThread = new Thread() {
                                           public void run() {

                                               Log.d(TAG, "Inserting data");
                                               ad.ExpenseDao().insert(new Expenses(typedname, typedCategory, typedDate, typedAmount, typedNote));
                                           }
                                       };
                                       myThread.start();
    }

    @Override
    public void updateTexts(final String typedname,final String typedCategory, final String typedDate, final double typedAmount,final  String typedNote, final String typedID)
    {

        Thread myThread = new Thread() {
            public void run() {

               long id = Long.parseLong(typedID);
                Expenses temp = new Expenses(typedname, typedCategory, typedDate, typedAmount, typedNote);
                temp.setId(id);
                ad.ExpenseDao().update(temp);
                Log.d(TAG, "Inserting data");
            }
        };
        myThread.start();
    }

    public void deleteExpense(final String typedname)
    {
        Thread myThread = new Thread() {
            public void run() {

                long id = Long.parseLong(typedname);
                Log.d(TAG, "Deleting data");
                ad.ExpenseDao().deleteById(id);
            }
        };
        myThread.start();
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        FloatingActionButton fab = findViewById(R.id.fab);
        FloatingActionButton deletefab = findViewById(R.id.deleteFab);
        fab.setOnClickListener(new View.OnClickListener()
                               {
                                   @Override
                                   public void onClick(View view) {
                                       openDialog();
                                   }
                               }
        );
        deletefab.setOnClickListener(new View.OnClickListener()
                               {
                                   @Override
                                   public void onClick(View view) {

                                       openDeleteDialog();

                                   }
                               }
        );
        ad = AppDatabase.getInstance(this);
        recycle = findViewById(R.id.rview);
        recycle.setLayoutManager(new LinearLayoutManager(this));
        recycle.setItemAnimator(new DefaultItemAnimator());
        final ExpenseListViewModel expenseListViewModel =  new ExpenseListViewModel(ad);
        final myAdapter mAdapter = new myAdapter(expenseListViewModel, R.layout.full_expense, this);

        recycle.setAdapter(mAdapter);


        expenseListViewModel.getExpenses().observe(this, new Observer<List<Expenses>>()
        {
            @Override
            public void onChanged(@Nullable List<Expenses> expenses)
            {
                mAdapter.setExpenses(expenses);
            }
        });

        expenseListViewModel.getExpenses().observe(this, new Observer<List<Expenses>>() {
            @Override
            public void onChanged(List<Expenses> expenses) {
                Log.d(TAG, "Data has been added/changed, displaying");
            }
        });
        mAdapter.setOnItemClickListener(new myAdapter.onItemClickListener() {
            @Override
            public void onItemClick(Expenses expense) {
                Uname = expense.getName();
                Ucat = expense.getCategory();
                Udate = expense.getDate();
                Uprice = String.valueOf(expense.getAmount());
                Unote = expense.getNote();
                UID = String.valueOf(expense.getId());
                openUpdateDialog();
            }
        });




    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        //getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();


        return super.onOptionsItemSelected(item);
    }

    public void openDialog(){
        Dialog dialog = new Dialog();
        dialog.show(getSupportFragmentManager(),"Dialog" );
    }
    public void openUpdateDialog(){
        UpdateDialog dialog = new UpdateDialog();
        dialog.show(getSupportFragmentManager(),"Dialog" );
    }
    public void openDeleteDialog(){
        deleteDialog dialog = new deleteDialog();
        dialog.show(getSupportFragmentManager(),"Dialog" );
    }
}
