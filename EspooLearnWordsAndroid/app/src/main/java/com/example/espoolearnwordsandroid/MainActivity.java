package com.example.espoolearnwordsandroid;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.ListView;

import java.util.ArrayList;

public class MainActivity extends AppCompatActivity
{

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        ArrayList userList = getListData();
        final ListView lv = (ListView)findViewById(R.id.wordslist);
        lv.setAdapter(new CustomListAdapter(this, userList));

    }

    private ArrayList getListData()
    {
        ArrayList<ListItem> results = new ArrayList<>();

        ListItem word1 = new ListItem();
        {
            word1.setPolishWord("adidasy");
            word1.setEnglishWord("trainers");
            word1.setPronunciation("[treinerz]");
        }
        results.add(word1);

        ListItem word2 = new ListItem();
        {
            word2.setPolishWord("wymowa");
            word2.setEnglishWord("pronunciation");
            word2.setPronunciation("[prenansiejszn]");
        }
        results.add(word2);

        ListItem word3 = new ListItem();
        {
            word3.setPolishWord("ograniczenia");
            word3.setEnglishWord("constraints");
            word3.setPronunciation("[kenstrejnts]");
        }
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);

        results.add(word1);
        results.add(word2);
        results.add(word3);




        return results;
    }
}
























