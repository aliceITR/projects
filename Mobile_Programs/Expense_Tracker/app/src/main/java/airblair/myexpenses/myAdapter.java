package airblair.myexpenses;

import androidx.lifecycle.Observer;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;
import androidx.fragment.app.Fragment;
import java.util.List;

public class myAdapter extends RecyclerView.Adapter<myAdapter.ViewHolder>{
    private List<Expenses> myList;
    private int rowLayout;
    private final String TAG= "myAdapter";
    private ExpenseListViewModel mViewModel;
    private onItemClickListener listener;

    // Provide a reference to the views for each data item
    // Complex data items may need more than one view per item, and
    // you provide access to all the views for a data item in a view holder

    public  class ViewHolder extends RecyclerView.ViewHolder {
        public TextView mName;
        public TextView mCategory;
        public TextView mDate;
        public TextView mAmount;
        public TextView mNote;
        public TextView mId;

        private final String TAG= "ViewHolder";
        public ViewHolder(View view) {
            super(view);
            mName = view.findViewById(R.id.nameBox);
            mCategory = view.findViewById(R.id.categoryBox);
            mDate = view.findViewById(R.id.dateBox);
            mAmount = view.findViewById(R.id.amountBox);
            mNote = view.findViewById(R.id.noteBox);
            mId = view.findViewById(R.id.iDBox);

            view.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    int position = getAdapterPosition();

                    if (listener != null && position != RecyclerView.NO_POSITION)
                    {
                        listener.onItemClick(myList.get(position));
                    }
                }
            });



        }
    }

    //constructor
    public myAdapter(ExpenseListViewModel expenseListViewModel, int rowLayout, AppCompatActivity activity) {
        mViewModel = expenseListViewModel;
        this.rowLayout = rowLayout;
        mViewModel.getExpenses().observe(activity, new Observer<List<Expenses>>() {
            @Override
            public void onChanged(@Nullable List<Expenses> scores) {
                Log.d("ntAdapter", "Data has been added/changed, displaying");
                myList = scores;
                notifyDataSetChanged();
            }
        });
    }

    // Create new views (invoked by the layout manager)
    @Override
    public ViewHolder onCreateViewHolder(ViewGroup viewGroup, int i) {
        View v = LayoutInflater.from(viewGroup.getContext()).inflate(rowLayout, viewGroup, false);
        return new ViewHolder(v);
    }


    public void setExpenses(List<Expenses> expenses)
    {
        myList = expenses;
        notifyDataSetChanged();
    }
    // Replace the contents of a view (invoked by the layout manager)
    @Override
    public void onBindViewHolder(ViewHolder viewHolder, int i) {
        Expenses entry = myList.get(i);

        viewHolder.mName.setText(entry.getName());
        viewHolder.mName.setTag(i);  //sample data to show.
        viewHolder.mCategory.setText(String.valueOf(entry.getCategory()));
        viewHolder.mDate.setText(String.valueOf(entry.getDate()));
        viewHolder.mAmount.setText(String.valueOf(entry.getAmount()));
        viewHolder.mNote.setText(String.valueOf(entry.getNote()));
        viewHolder.mId.setText(String.valueOf(entry.getId()));
    }

    // Return the size of your dataset (invoked by the layout manager)
    @Override
    public int getItemCount() {
        return myList == null ? 0 : myList.size();
    }
    public Expenses getExpenseAt(int position)
    {
        return myList.get(position);
    }

//

    public interface onItemClickListener
    {
        void onItemClick(Expenses expense);
    }

    public void setOnItemClickListener(onItemClickListener listener)
    {
        this.listener = listener;
    }



}
