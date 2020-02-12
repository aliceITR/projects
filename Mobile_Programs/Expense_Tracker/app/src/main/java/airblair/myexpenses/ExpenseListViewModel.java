package airblair.myexpenses;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MediatorLiveData;
import androidx.lifecycle.ViewModel;
import androidx.annotation.Nullable;
import java.util.List;

public class ExpenseListViewModel extends ViewModel {

    private final MediatorLiveData<List<Expenses>> mObservableExpenses;

    private AppDatabase ad;
    public ExpenseListViewModel(AppDatabase ad) {

        mObservableExpenses = new MediatorLiveData<>();
        mObservableExpenses.setValue(null);
        LiveData<List<Expenses>> scores = ad.ExpenseDao().selectAll();
        mObservableExpenses.addSource(scores, new androidx.lifecycle.Observer<List<Expenses>>() {
                    @Override
                    public void onChanged(@Nullable List<Expenses> scoreEntities) {
                        mObservableExpenses.setValue(scoreEntities);
                    }
                }
        );
    }
    public LiveData<List<Expenses>> getExpenses() {
        return mObservableExpenses;
    }

    public void insert(Expenses expense)
    {
        ad.ExpenseDao().insert(expense);
    }

    public void update(Expenses expense)
    {
        ad.ExpenseDao().update(expense);
    }

    public void delete(Expenses expense)
    {
        ad.ExpenseDao().delete(expense);
    }


}
