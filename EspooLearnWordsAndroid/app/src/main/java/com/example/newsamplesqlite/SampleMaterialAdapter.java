package com.example.newsamplesqlite;

import android.animation.Animator;
import android.animation.AnimatorListenerAdapter;
import android.content.Context;
import android.content.Intent;
import android.os.AsyncTask;
import androidx.core.util.Pair;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewAnimationUtils;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityOptionsCompat;
import androidx.recyclerview.widget.RecyclerView;

import java.util.ArrayList;

public class SampleMaterialAdapter extends RecyclerView.Adapter<SampleMaterialAdapter.ViewHolder> {
    private static final String DEBUG_TAG = "SampleMaterialAdapter";

    private Context context;
    private ArrayList<Card> cardsList;
    public CardsData cardsData;

    public SampleMaterialAdapter(Context context, ArrayList<Card> cardsList, CardsData cardsData) {
        this.context = context;
        this.cardsList = cardsList;
        this.cardsData = cardsData;
    }

    @Override
    public void onBindViewHolder(ViewHolder viewHolder, int position)
    {
        String name = cardsList.get(position).getWordEnglish();
        String word1pronun = cardsList.get(position).getWordPronun();
        String word_2 = cardsList.get(position).getWordForeign();
        int color = cardsList.get(position).getColorResource();

        TextView english_word = viewHolder.english_word;
        TextView word_1_pronun_tv = viewHolder.word_1_pronun;
        TextView word_2_tv = viewHolder.word_2;
        //TextView nameTextView = viewHolder.name;

        english_word.setText(name);
        english_word.setBackgroundColor(color);
        word_1_pronun_tv.setText(word1pronun);
        word_1_pronun_tv.setBackgroundColor(color);
        word_2_tv.setText(word_2);
        word_2_tv.setBackgroundColor(color);
        //nameTextView.setText(name);
    }

    @Override
    public void onViewDetachedFromWindow(ViewHolder viewHolder) {
        super.onViewDetachedFromWindow(viewHolder);
        viewHolder.itemView.clearAnimation();
    }

    @Override
    public void onViewAttachedToWindow(ViewHolder viewHolder) {
        super.onViewAttachedToWindow(viewHolder);
        //animateCircularReveal(viewHolder.itemView);
    }

    public void animateCircularReveal(View view) {
        int centerX = 0;
        int centerY = 0;
        int startRadius = 0;
        int endRadius = Math.max(view.getWidth(), view.getHeight());
        Animator animation = ViewAnimationUtils.createCircularReveal(view, centerX, centerY, startRadius, endRadius);
        view.setVisibility(View.VISIBLE);
        animation.start();
    }

    public void animateCircularDelete(final View view, final int list_position) {
        int centerX = view.getWidth();
        int centerY = view.getHeight();
        int startRadius = view.getWidth();
        int endRadius = 0;
        Animator animation = ViewAnimationUtils.createCircularReveal(view, centerX, centerY, startRadius, endRadius);

        animation.addListener(new AnimatorListenerAdapter() {
            @Override
            public void onAnimationEnd(Animator animation) {
                super.onAnimationEnd(animation);
                view.setVisibility(View.INVISIBLE);
                Card card = new Card();
                card.setId(getItemId(list_position));
                card.setListPosition(list_position);
                new DeleteCardTask().execute(card);
            }
        });
        animation.start();
    }

    public void addCard(String name, String word1pron, String word2, int color) {
        Card card = new Card();
        card.setWordEnglish(name);
        card.setWordPronun(word1pron);
        card.setWordForeign(word2);
        card.setColorResource(color);
        new CreateCardTask().execute(card);
    }

    public void updateCard(String englishWord, int list_position) {
        Card card = new Card();
        card.setWordEnglish(englishWord);
        card.setId(getItemId(list_position));
        card.setListPosition(list_position);
        new UpdateCardTask().execute(card);
    }

    public void updateCard(String englishWord, String pronun, String foreign, int list_position) {
        Log.d(DEBUG_TAG, "updateCard()");
        Log.d(DEBUG_TAG, "englishWord=" + englishWord + " pronun=" + pronun + " foreign=" + foreign);
        Card card = new Card();
        card.setWordEnglish(englishWord);
        card.setWordPronun(pronun);
        card.setWordForeign(foreign);
        card.setId(getItemId(list_position));
        card.setListPosition(list_position);
        new UpdateCardTask().execute(card);
    }

    public void deleteCard(View view, int list_position) {
        animateCircularDelete(view, list_position);
    }

    @Override
    public int getItemCount() {
        if (cardsList.isEmpty()) {
            return 0;
        } else {
            return cardsList.size();
        }
    }

    @Override
    public long getItemId(int position) {
        return cardsList.get(position).getId();
    }

    @Override
    public ViewHolder onCreateViewHolder(ViewGroup viewGroup, int i) {
        LayoutInflater li = LayoutInflater.from(viewGroup.getContext());
        View v = li.inflate(R.layout.card_view_holder, viewGroup, false);
        return new ViewHolder(v);
    }

    public class ViewHolder extends RecyclerView.ViewHolder
    {
        private TextView english_word;
        private TextView word_1_pronun;
        private TextView word_2;
//        private Button deleteButton;

        public ViewHolder(View v)
        {
            super(v);
            english_word = (TextView) v.findViewById(R.id.english_word);
            word_1_pronun = (TextView) v.findViewById(R.id.word_1_pronun);
            word_2 = (TextView)  v.findViewById(R.id.word_2);
//            deleteButton = (Button) v.findViewById(R.id.delete_button);
//
//            deleteButton.setOnClickListener(new View.OnClickListener() {
//                @Override
//                public void onClick(View v) {
//                    animateCircularDelete(itemView, getAdapterPosition());
//                }
//            });

            itemView.setOnLongClickListener(new View.OnLongClickListener() {
                @Override
                public boolean onLongClick(View v)
                {
                    Pair<View, String> p1 = Pair.create((View) english_word, SampleMaterialActivity.TRANSITION_ENGLISH_WORD);
                    Pair<View, String> p2 = Pair.create((View) word_1_pronun, SampleMaterialActivity.TRANSITION_PRONUN);
                    Pair<View, String> p3 = Pair.create((View) word_2, SampleMaterialActivity.TRANSITION_FOREIGN);

                    ActivityOptionsCompat options;
                    AppCompatActivity act = (AppCompatActivity) context;

                    options = ActivityOptionsCompat.makeSceneTransitionAnimation(act, p1, p2, p3);

                    int requestCode = getAdapterPosition();
                    String englishWord = cardsList.get(requestCode).getWordEnglish();
                    String proun = cardsList.get(requestCode).getWordPronun();
                    String foreign = cardsList.get(requestCode).getWordForeign();
                    int color = cardsList.get(requestCode).getColorResource();

                    Log.d(DEBUG_TAG, "SampleMaterialAdapter klinięto w celu edycji, pozycja elementu: " + requestCode);

                    Intent transitionIntent = new Intent(context, TransitionEditActivity.class);
                    transitionIntent.putExtra(SampleMaterialActivity.EXTRA_ENGLISH_WORD, englishWord);
                    transitionIntent.putExtra(SampleMaterialActivity.EXTRA_PRONUN, proun);
                    transitionIntent.putExtra(SampleMaterialActivity.EXTRA_FOREIGN, foreign);
                    transitionIntent.putExtra(SampleMaterialActivity.EXTRA_INITIAL, englishWord);
                    transitionIntent.putExtra(SampleMaterialActivity.EXTRA_COLOR, color);
                    transitionIntent.putExtra(SampleMaterialActivity.EXTRA_UPDATE, false);
                    transitionIntent.putExtra(SampleMaterialActivity.EXTRA_DELETE, false);
                    ((AppCompatActivity) context).startActivityForResult(transitionIntent, requestCode, options.toBundle());

                    return true;
                }

            });
        }
    }

    private class CreateCardTask extends AsyncTask<Card, Void, Card> {
        @Override
        protected Card doInBackground(Card... cards) {
            cardsData.create(cards[0]);
            cardsList.add(cards[0]);
            return cards[0];
        }

        @Override
        protected void onPostExecute(Card card) {
            super.onPostExecute(card);
            ((SampleMaterialActivity) context).doSmoothScroll(getItemCount() - 1);
            notifyItemInserted(getItemCount());
            Log.d(DEBUG_TAG, "Utworzono kartę o identyfikatorze " + card.getId() + ", imię: " + card.getWordEnglish() + ", kolor: " + card.getColorResource());
        }
    }

    private class UpdateCardTask extends AsyncTask<Card, Void, Card> {
        @Override
        protected Card doInBackground(Card... cards) {
            long position = cards[0].getId();
            String english_word = cards[0].getWordEnglish();
            String pronun = cards[0].getWordPronun();
            String foreign_word = cards[0].getWordForeign();

            cardsData.update(position, english_word, pronun, foreign_word);
            cardsList.get(cards[0].getListPosition()).setWordEnglish(english_word);
            cardsList.get(cards[0].getListPosition()).setWordPronun(pronun);
            cardsList.get(cards[0].getListPosition()).setWordForeign(foreign_word);
            return cards[0];
        }

        @Override
        protected void onPostExecute(Card card) {
            super.onPostExecute(card);
            Log.d(DEBUG_TAG, "list_position ma wartość " + card.getListPosition());
            notifyItemChanged(card.getListPosition());
        }
    }

    private class DeleteCardTask extends AsyncTask<Card, Void, Card> {
        @Override
        protected Card doInBackground(Card... cards) {
            cardsData.delete(cards[0].getId());
            cardsList.remove(cards[0].getListPosition());
            return cards[0];
        }

        @Override
        protected void onPostExecute(Card card) {
            super.onPostExecute(card);
            notifyItemRemoved(card.getListPosition());
        }
    }
}
