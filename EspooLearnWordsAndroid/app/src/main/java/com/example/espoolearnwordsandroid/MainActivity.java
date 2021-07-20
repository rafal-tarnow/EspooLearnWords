package com.example.espoolearnwordsandroid;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.ArrayAdapter;
import android.widget.ListView;

public class MainActivity extends AppCompatActivity {

    private ListView mListView;
    private ArrayAdapter aAdapter;

    private String[] words = { "ograniczenia constraints", "ograniczenia constraints", "ograniczenia constraints", "ograniczenia constraints", "ograniczenia constraints", "ograniczenia constraints", "ograniczenia constraints", "ograniczenia constraints", "ograniczenia constraints", "ograniczenia constraints", "ograniczenia constraints", "ograniczenia constraints", "ograniczenia constraints", "ograniczenia constraints", "Rohini Alavala", "Trishika Dasari", "Praveen Alavala", "Madav Sai", "Hamsika Yemineni"};

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mListView = (ListView) findViewById(R.id.wordslist);
        aAdapter = new ArrayAdapter(this, android.R.layout.simple_list_item_1, words);
        mListView.setAdapter(aAdapter);
    }
}