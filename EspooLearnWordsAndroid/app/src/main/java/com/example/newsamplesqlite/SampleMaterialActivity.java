package com.example.newsamplesqlite;

import android.app.Activity;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import androidx.core.util.Pair;

import android.util.Log;
import android.view.View;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityOptionsCompat;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.google.android.material.floatingactionbutton.FloatingActionButton;

import java.util.ArrayList;

public class SampleMaterialActivity extends AppCompatActivity {
    private static final String DEBUG_TAG = "AppCompatActivity";

    public static final String EXTRA_UPDATE = "update";
    public static final String EXTRA_DELETE = "delete";
    public static final String EXTRA_ENGLISH_WORD = "name";
    public static final String EXTRA_PRONUN = "word1pronun";
    public static final String EXTRA_FOREIGN = "word2";
    public static final String EXTRA_INITIAL = "initial";
    public static final String EXTRA_COLOR = "color";


    public static final String TRANSITION_FAB = "fab_transition";
    public static final String TRANSITION_ENGLISH_WORD = "english_word_transition";
    public static final String TRANSITION_PRONUN = "pronun_transition";
    public static final String TRANSITION_FOREIGN = "foreign_transition";

    public RecyclerView recyclerView;
    private SampleMaterialAdapter adapter;
    private ArrayList<Card> cardsList = new ArrayList<>();
    private int[] colors;
    //private String[] names;
    public CardsData cardsData = new CardsData(this);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_sample_material);

        colors = getResources().getIntArray(R.array.initial_colors);

        recyclerView = (RecyclerView) findViewById(R.id.recycler_view);
        recyclerView.setLayoutManager(new LinearLayoutManager(this));

        new GetCardsListTask().execute();

        FloatingActionButton fab = (FloatingActionButton) findViewById(R.id.fab);
        fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Pair<View, String> pair = Pair.create(v.findViewById(R.id.fab), TRANSITION_FAB);

                ActivityOptionsCompat options;
                Activity act = SampleMaterialActivity.this;
                options = ActivityOptionsCompat.makeSceneTransitionAnimation(act, pair);

                Intent transitionIntent = new Intent(act, TransitionAddActivity.class);
                act.startActivityForResult(transitionIntent, adapter.getItemCount(), options.toBundle());
            }
        });
    }

    @Override
    protected void onResume() {
        super.onResume();
        cardsData.open();
        Log.d(DEBUG_TAG, "cardsData otworzno w metodzie onResume()");
    }

    @Override
    protected void onPause() {
        super.onPause();
        cardsData.close();
        Log.d(DEBUG_TAG, "cardsData zamkni??to w metodzie onPause()");
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        cardsData.open();
        Log.d(DEBUG_TAG, "requestCode ma warto???? " + requestCode);
        // Je??li wywo??anie adapter.getItemCount() zar??ci kod ????dania, b??dzie to oznacza??, ??e dodajemy now?? pozycj??.
        // Wszelkie zwr??cone warto??ci mniejsze od adapter.getItemCount() oznaczaj?? edycj?? istniej??cego elementu.
        if (requestCode == adapter.getItemCount()) {
            if (resultCode == RESULT_OK) {
                // Upewniamy si??, ??e ????danie dodania zako??czy??o si?? pomy??lnie. W takim przypadku
                // dodajemy imi?? i aktualizujemy list??.
                String name = data.getStringExtra(EXTRA_ENGLISH_WORD);
                String word1pro = data.getStringExtra(EXTRA_PRONUN);
                String word2 = data.getStringExtra(EXTRA_FOREIGN);
                int color = data.getIntExtra(EXTRA_COLOR, 0);

                int newPosition = adapter.getItemCount();
                Log.d(DEBUG_TAG, "newPosition = " + newPosition);
                adapter.addCard(name, word1pro, word2, color);
            }
        } else {
            // Jakakolwiek warto??c r????na od adapter.getItemCount() oznacza edycj?? istniej??cego elementu listy,
            // warto???? requestCode okre??la po??o??enie elementu na li??cie.
            if (resultCode == RESULT_OK) {
                // Upewniamy si??, ??e ????danie aktualizacji zosta??o wykonane prawid??owo.
                RecyclerView.ViewHolder viewHolder = recyclerView.findViewHolderForAdapterPosition(requestCode);
                if (data.getExtras().getBoolean(EXTRA_DELETE, false)) {
                    // Je??li za????dano usuni??cia, to usuwamy.
                    // U??ytkownik usun???? kontakt.
                    adapter.deleteCard(viewHolder.itemView, requestCode);
                } else if (data.getExtras().getBoolean(EXTRA_UPDATE)) {
                    // Imi?? uleg??o zmianie, aktualizujemy u??ytkownika.
                    String name = data.getStringExtra(EXTRA_ENGLISH_WORD);
                    String pronun = data.getStringExtra(EXTRA_PRONUN);
                    String foreign = data.getStringExtra(EXTRA_FOREIGN);
                    viewHolder.itemView.setVisibility(View.INVISIBLE);
                    adapter.updateCard(name, pronun, foreign, requestCode);
                }
            }
        }
    }

    public void doSmoothScroll(int position) {
        recyclerView.smoothScrollToPosition(position);
    }

    public class GetCardsListTask extends AsyncTask<Void, Void, ArrayList<Card>> {
        @Override
        protected ArrayList<Card> doInBackground(Void... params) {
            cardsData.open();
            cardsList = cardsData.getAll();
            return cardsList;
        }

        @Override
        protected void onPostExecute(ArrayList<Card> cards) {
            super.onPostExecute(cards);
            adapter = new SampleMaterialAdapter(SampleMaterialActivity.this, cardsList, cardsData);
            recyclerView.setAdapter(adapter);
        }
    }
}












